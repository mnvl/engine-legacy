
#include <set>
#include <map>
#include <string>
#include <sstream>
#include <memory>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <util/xerces_lcp_transcoder.h>
#include <util/logger.h>
#include <math/matrix.h>
#include "collada_loader.h"

#if defined(max)
#undef max
#endif

namespace collada_elements
{
	using namespace util;
	using namespace graphic;

	class mesh_e;
	class library_visual_scenes_e;
	class library_geometries_e;
	class library_controllers_e;
	class library_materials_e;
	class library_effects_e;
	class library_images_e;

	class root_element {
	public:
		virtual Model *get_model() const = 0;
		virtual library_visual_scenes_e *get_visual_scenes() const = 0;
		virtual library_geometries_e *get_geometries() const = 0;
		virtual library_controllers_e *get_controllers() const = 0;
		virtual library_materials_e *get_materials() const = 0;
		virtual library_effects_e *get_effects() const = 0;
		virtual library_images_e *get_images() const = 0;
		virtual boost::filesystem::path const &get_basepath() const = 0;
	};

	class element {
	private:
		root_element *root_;

	public:
		element():
		root_(0)
		{
		}

		void go(xercesc::DOMElement const *element_ptr) {
			for (xercesc::DOMNode const *child_ptr = element_ptr->getFirstChild();
				child_ptr != 0; child_ptr = child_ptr->getNextSibling())
			{
				if (child_ptr->getNodeType() == xercesc::DOMNode::ELEMENT_NODE) {
					process_child(xerces_lcp_transcode(child_ptr->getLocalName()),
						xerces_get_attribute<std::string>(child_ptr, "id"), (xercesc::DOMElement *) child_ptr);
				}
			}
		}

		root_element *get_root() const { return root_; }
		Model *get_model() const { return root_->get_model(); }
		void set_root(root_element *root) { root_ = root; }

		void init(xercesc::DOMElement const *element_ptr) {}
		virtual void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child_ptr) {}
		void check() {}

		template <class T> T *
		construct(xercesc::DOMElement const *element_ptr) {
			std::auto_ptr<T> holder(new T());
			holder->set_root(get_root());
			holder->init(element_ptr);
			holder->go(element_ptr);
			holder->check();
			return holder.release();
		}

		template<class T> static void
		parse_list(xercesc::DOMElement const *element_ptr, std::vector<T> &v)
		{
			std::stringstream ss(xerces_get_text(element_ptr));
			T dummy;
			while (ss >> dummy) v.push_back(dummy);
			if (ss.bad()) throw std::logic_error("can't parse list");
		}

		template<class T> static size_t
		parse_list(xercesc::DOMElement const *element_ptr, T *list_ptr, size_t size)
		{
			std::stringstream ss(xerces_get_text(element_ptr));
			size_t i = 0;
			while (i < size && ss >> *list_ptr++) i++;
			return i;
		}

		void parse_matrix(xercesc::DOMElement const *element_ptr, math::matrix<4, 4> &tf)
		{
			if (parse_list(element_ptr, &tf.ij[0][0], 16) != 16) throw std::logic_error("invalid <matrix>");
			tf.transpose_inplace();
		}
	};

	class technique_common_e: public element {
	private:
		struct param {
			std::string name;
			std::string type;

			param(std::string const &n, std::string const &t):
			name(n),
				type(t)
			{
			}
		};

		size_t count_, stride_;
		std::vector<param> params_;
		std::map<std::string, std::string> instance_materials_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child_ptr)
		{
			if (child_name == "accessor")
			{
				count_ = xerces_get_attribute<size_t>(child_ptr, "count");
				stride_ = xerces_get_attribute<size_t>(child_ptr, "stride", 3);

				xercesc::DOMNodeList const *params = child_ptr->getElementsByTagName(xerces_lcp_transcode("param").c_str());

				for (size_t i = 0, n = params->getLength(); i < n; i++)
				{
					xercesc::DOMNode const *node_e = params->item(i);
					params_.push_back(param(xerces_get_attribute<std::string>(node_e, "name"), xerces_get_attribute<std::string>(node_e, "type")));
				}
			}
			else if (child_name == "instance_material")
			{
				std::string const &symbol = xerces_get_attribute<std::string>(child_ptr, "symbol");
				std::string const &target = xerces_get_attribute<std::string>(child_ptr, "target");
				instance_materials_.insert(std::make_pair(symbol, target.substr(1)));
			}
		}

	public:
		size_t get_stride() const { return stride_; }
		size_t get_count() const { return count_; }

		std::string const &get_material_by_symbol(std::string const &symbol) const
		{
			std::map<std::string, std::string>::const_iterator it = instance_materials_.find(symbol);
			if (it != instance_materials_.end()) return it->second;

			throw std::logic_error("<instance_material> with symbol = " + symbol + " not found");
		}
	};

	class source_e: public element {
	private:
		std::vector<float> float_array_;
		std::vector<std::string> name_array_;
		boost::scoped_ptr<technique_common_e> technique_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child_ptr) {
			if (child_name == "float_array") {
				parse_list(child_ptr, float_array_);
			}
			else if (child_name == "Name_array") {
				parse_list(child_ptr, name_array_);
			}
			else if (child_name == "technique_common") {
				technique_.reset(construct<technique_common_e>(child_ptr));
			}
		}

	public:
		technique_common_e const &get_technique() const { return *technique_; }

		void check() {
			if (float_array_.empty() && name_array_.empty()) throw std::logic_error("malformed or unknown <source>");
			if (!technique_) throw std::logic_error("no <technique_common> in <source>");

			size_t array_size = std::max(float_array_.size(), name_array_.size());
			size_t expected_size = technique_->get_stride() * technique_->get_count();

			if (array_size != expected_size)
			{
				throw std::logic_error("array has " + boost::lexical_cast<std::string>(expected_size) 
					+ " elements, parsed " + boost::lexical_cast<std::string>(array_size) + " elements");
			}

		}

		size_t get_size() const {
			return std::max(name_array_.size(), float_array_.size());
		}

		void get(size_t index, std::string &str) const {
			str = name_array_.at(index);
		}

		void get(size_t index, float &f) const {
			f = float_array_.at(index);
		}

		template <int N>
		void get(size_t index, math::vec<N> &v) const {
			size_t array_index = index * technique_->get_stride();

			for (size_t i = 0; i < N; i++) {
				v.i[i] = float_array_.at(array_index++);
			}
		}

		void get(size_t index, math::matrix<4, 4> &tf) const {
			size_t array_index = index * technique_->get_stride();

			for (size_t i = 0; i < 4; i++) {
				for (size_t j = 0; j < 4; j++) {
					tf.ij[j][i] = float_array_.at(array_index++);
				}
			}
		}
	};

	class sources_list_e: public element {
	protected:
		boost::ptr_map<std::string, source_e> sources_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child_ptr) {
			if (child_name == "source") {
				sources_.insert(const_cast<std::string &>(child_id), construct<source_e>(child_ptr));
			}
		}

	public:
		source_e const *get_source(std::string const &id) const {
			boost::ptr_map<std::string, source_e>::const_iterator it = sources_.find(id);
			if (it == sources_.end()) throw std::logic_error("source id = " + id + " not found");
			return it->second;
		}
	};

	class transform_e: public element {
	protected:
		std::string sid_;

	public:
		void init(xercesc::DOMElement const *element_ptr)
		{
			sid_ = xerces_get_attribute<std::string>(element_ptr, "sid");
		}

		std::string const &get_sid() const { return sid_; }

		virtual math::matrix<4,4> get_baked_matrix() const = 0;
		virtual math::matrix<4,4> get_baked_sample(source_e const *source_ptr, int index, std::string const &param) const = 0;
	};

	class identity_transform_e: public transform_e {
	public:
		math::matrix<4,4> get_baked_matrix() const
	    {
			math::matrix<4,4> m;
			m.identity();
			return m;
		}

		math::matrix<4,4> get_baked_sample(source_e const *source_ptr, int index, std::string const &param) const
		{
			throw std::logic_error("can't read identity transform sample from source");
	    }
	};


	class matrix_transform_e: public transform_e {
	private:
		math::matrix<4,4> matrix_;

	public:
		void init(xercesc::DOMElement const *element_ptr)
		{
			transform_e::init(element_ptr);
			parse_matrix(element_ptr, matrix_);
		}

		math::matrix<4,4> get_baked_matrix() const
		{
			return matrix_;
		}

		math::matrix<4,4> get_baked_sample(source_e const *source_ptr, int index, std::string const &param) const
		{
			math::matrix<4,4> result;
			source_ptr->get(index, result);
			return result;
		}
	};

	class translation_transform_e: public transform_e {
	private:
		math::vec<3> translate_;

	public:
		void init(xercesc::DOMElement const *element_ptr)
		{
			transform_e::init(element_ptr);
			parse_list(element_ptr, translate_.i, 3);
		}

	    math::matrix<4,4> get_baked_matrix() const
		{
			math::matrix<4,4> result;
			result.translation(translate_);
			return result;
		}

		math::matrix<4,4> get_baked_sample(source_e const *source_ptr, int index, std::string const &param) const
		{
			math::vec<3> translate;
			source_ptr->get(index, translate);

			math::matrix<4,4> result;
			result.translation(translate);

			return result;
		}
	};

	class rotation_transform_e: public transform_e {
	private:
		math::vec<3> axis_;
		math::scalar angle_;

	public:
		void init(xercesc::DOMElement const *element_ptr)
		{
			transform_e::init(element_ptr);

			math::scalar s[4];
			parse_list(element_ptr, s, 4);

			axis_.set(s[0], s[1], s[2]);
			angle_ = s[3];
		}

		math::matrix<4,4> get_baked_matrix() const
		{
			math::matrix<4,4> result;
			result.rotation(axis_, angle_ * math::PI / 180.0f);
			return result;
		}

		math::matrix<4,4> get_baked_sample(source_e const *source_ptr, int index, std::string const &param) const
		{
			if (param != "ANGLE") throw std::logic_error("unexpected rotation transform param type " + param);

			math::scalar angle;
			source_ptr->get(index, angle);

			math::matrix<4,4> result;
			result.rotation(axis_, angle * math::PI / 180.0f);

			return result;
		}
	};

	class scale_transform_e: public transform_e {
	private:
		math::vec<3> scale_;

	public:
		void init(xercesc::DOMElement const *element_ptr)
		{
			transform_e::init(element_ptr);
			parse_list(element_ptr, scale_.i, 3);
		}

		math::matrix<4,4> get_baked_matrix() const
		{
			math::matrix<4,4> result;
			result.scaling(scale_);
			return result;
		}

		math::matrix<4,4> get_baked_sample(source_e const *source_ptr, int index, std::string const &param) const
		{
			math::vec<3> scale;
			source_ptr->get(index, scale);

			math::matrix<4,4> result;
			result.scaling(scale);

			return result;
		}
	};

	class bind_material_e: public element {
	private:
		boost::scoped_ptr<technique_common_e> technique_common_ptr_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child_ptr)
		{
			if (child_name == "technique_common")
			{
				technique_common_ptr_.reset(construct<technique_common_e>(child_ptr));
			}
		}

	public:
		void bind_mesh_materials(mesh_e const *, MeshPtr mesh_ptr) const;

		technique_common_e const *get_technique_common() const
		{
			return technique_common_ptr_.get();
		}
	};

	class instance_controller_e: public element {
	private:
		std::string url_;
		std::string skeleton_;
		boost::scoped_ptr<bind_material_e> bind_material_ptr_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child_ptr)
		{
			if (child_name == "skeleton")
			{
				skeleton_ = xerces_get_text(child_ptr).substr(1);
			}
			else if (child_name == "bind_material")
			{
				bind_material_ptr_.reset(construct<bind_material_e>(child_ptr));
			}
		}

	public:
		std::string const &get_url() const { return url_; }
		std::string const &get_skeleton() const { return skeleton_; }

		bind_material_e const *get_bind_material() const
		{
			return bind_material_ptr_.get();
		}

		void init(xercesc::DOMElement const *element_ptr)
		{
			url_ = xerces_get_attribute<std::string>(element_ptr, "url").substr(1);
		}
	};

	class instance_geometry_e: public element {
	private:
		std::string url_;
		boost::scoped_ptr<bind_material_e> bind_material_ptr_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child_ptr)
		{
			if (child_name == "bind_material")
			{
				bind_material_ptr_.reset(construct<bind_material_e>(child_ptr));
			}
		}

	public:
		std::string const &get_url() const
		{
			return url_;
		}

		bind_material_e const *get_bind_material() const
		{
			return bind_material_ptr_.get();
		}

		void init(xercesc::DOMElement const *element_ptr)
		{
			url_ = xerces_get_attribute<std::string>(element_ptr, "url").substr(1);
		}
	};

	class node_e: public element {
	protected:
		std::string id_, sid_, name_, type_;
		node_e *parent_;
		boost::ptr_vector<node_e> childs_;
		boost::ptr_vector<transform_e> transforms_;
		boost::scoped_ptr<instance_controller_e> instance_controller_;
		boost::scoped_ptr<instance_geometry_e> instance_geometry_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child_ptr)
		{
			if (child_name == "node")
			{
				childs_.push_back(construct<node_e>(child_ptr));
				childs_.back().parent_ = this;
			}
			else if (child_name == "matrix")
			{
				transforms_.push_back(construct<matrix_transform_e>(child_ptr));
			}
			else if (child_name == "translate")
			{
				transforms_.push_back(construct<translation_transform_e>(child_ptr));
			}
			else if (child_name == "rotate")
			{
				transforms_.push_back(construct<rotation_transform_e>(child_ptr));
			}
			else if (child_name == "scale")
			{
				transforms_.push_back(construct<scale_transform_e>(child_ptr));
			}
			else if (child_name == "instance_geometry")
			{
				instance_geometry_.reset(construct<instance_geometry_e>(child_ptr));
			}
			else if (child_name == "instance_controller")
			{
				instance_controller_.reset(construct<instance_controller_e>(child_ptr));
			}
		}

	public:
		void init(xercesc::DOMElement const *element_ptr) {
			id_ = xerces_get_attribute<std::string>(element_ptr, "id");
			sid_ = xerces_get_attribute<std::string>(element_ptr, "sid");
			name_ = xerces_get_attribute<std::string>(element_ptr, "name");
			type_ = xerces_get_attribute<std::string>(element_ptr, "type");
		}

		void check() {
			if (transforms_.empty()) {
				transforms_.push_back(new identity_transform_e());
			}
		}

		void build_hierarchy(JointPtr parent_ptr);

		node_e const *get_node_by_id(std::string const &id) const
		{
			if (id == id_) return this;

			for (boost::ptr_vector<node_e>::const_iterator it = childs_.begin(); it != childs_.end(); it++)
			{
				if (node_e const *node_ptr = it->get_node_by_id(id)) return node_ptr;
			}

			return 0;
		}

		transform_e const *get_transform_by_sid(std::string const &sid) const
		{
			for (boost::ptr_vector<transform_e>::const_iterator it = transforms_.begin();
				it != transforms_.end(); it ++)
			{
				if (it->get_sid() == sid) return &*it;
			}

			throw std::logic_error("transform with sid = " + sid + " not found in node id = " + id_);
		}
	};

	class visual_scene_e: public node_e {
	public:
		void check() {
		}

		void build_hierarchy() {
			for (boost::ptr_vector<node_e>::iterator it = childs_.begin(); it != childs_.end(); it++) {
				it->build_hierarchy(JointPtr());
			}
		}
	};

	class library_visual_scenes_e: public element {
	protected:
		boost::ptr_map<std::string, visual_scene_e> visual_scenes_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child_ptr) {
			if (child_name == "visual_scene") {
				visual_scenes_.insert(const_cast<std::string &>(child_id), construct<visual_scene_e>(child_ptr));
			}
		}

	public:
		node_e const *get_node_by_id(std::string const &id) const
		{
			for (boost::ptr_map<std::string, visual_scene_e>::const_iterator it = visual_scenes_.begin();
				it != visual_scenes_.end(); it++)
			{
				if (node_e const *node_ptr = it->second->get_node_by_id(id)) return node_ptr;
			}

			throw std::logic_error("node with id = " + id + " not found");
		}

		void build_hierarchy() {
			if (visual_scenes_.size() != 1) throw std::logic_error("expecting one <visual_scene> in <library_visual_scenes>");
			visual_scenes_.begin()->second->build_hierarchy();
		}
	};

	class input_e: public element {
	private:
		std::string source_;
		size_t offset_;

	public:
		void init(xercesc::DOMElement const *element_ptr) {
			source_ = xerces_get_attribute<std::string>(element_ptr, "source");
			offset_ = xerces_get_attribute<size_t>(element_ptr, "offset");
		}

		std::string const &get_source() const { return source_; }
		size_t get_offset() const { return offset_; }
	};

	class inputs_list_e: public element {
	protected:
		typedef boost::ptr_map<std::string, input_e> inputs_type;
		inputs_type inputs_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child_ptr) {
			if (child_name == "input") {
				inputs_.insert(xerces_get_attribute<std::string>(child_ptr, "semantic"), construct<input_e>(child_ptr));
			}
		}

	public:
		size_t get_stride() const
		{
			if (inputs_.empty()) throw std::logic_error("trying to calculate stride of empty input list");

			size_t max_offset = 0;

			for (inputs_type::const_iterator it = inputs_.begin(); it != inputs_.end(); it++)
			{
				if (it->second->get_offset() > max_offset) max_offset = it->second->get_offset(); 
			}

			return 1 + max_offset;
		}

		input_e const *get_input(std::string const &semantic) const
		{
			inputs_type::const_iterator it = inputs_.find(semantic);
			if (it == inputs_.end()) throw std::logic_error("input with semantic = " + semantic + " not found");
			return it->second;
		}
	};

	class vertices_e: public inputs_list_e {
	};

	class triangles_e: public inputs_list_e {
	private:
		size_t count_;
		std::vector<size_t> indices_;
		std::string material_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child_ptr)
		{
			if (child_name == "p")
			{
				parse_list(child_ptr, indices_);
			}

			inputs_list_e::process_child(child_name, child_id, child_ptr);
		}

	public:
		void init(xercesc::DOMElement const *element_ptr)
		{
			count_ = xerces_get_attribute<size_t>(element_ptr, "count");
			material_ = xerces_get_attribute<std::string>(element_ptr, "material");

			LOG_DEBUG("triangles count = " << count_ << ", material = " << material_);
		}

		size_t get_count() const { return count_; }
		std::string const &get_material() const { return material_; }
		std::vector<size_t> const &get_indices() const { return indices_; }
	};

	class vertex_indexer {
	public:
		struct vertex_t
		{
			math::vec<3> position;
			math::vec<3> normal;
			math::vec<2> texcoords;
		};

		std::pair<size_t, bool> choose_index(vertex_t const &v)
		{
			vertex_x_index_type::const_iterator it = vertex_x_index_.find(v);
			if (it != vertex_x_index_.end()) return std::pair<size_t, bool>(it->second, false);

			size_t index = vertex_x_index_.size();
			vertex_x_index_.insert(std::make_pair(v, index));
			return std::pair<size_t, bool>(index, true);
		}

	private:
		struct vertex_less {
			bool operator ()(vertex_t const &lhs, vertex_t const &rhs) const
            {
				if ((lhs.position - rhs.position).length_sq() > math::EPSILON)
				{
					if (math::lexicographical_less(lhs.position, rhs.position)) return true;
					if (math::lexicographical_greater(lhs.position, rhs.position)) return false;
				}

				if ((lhs.normal - rhs.normal).length_sq() > math::EPSILON)
				{
					if (math::lexicographical_less(lhs.normal, rhs.normal)) return true;
					if (math::lexicographical_greater(lhs.normal, rhs.normal)) return false;
				}

				if ((lhs.texcoords - rhs.texcoords).length_sq() > math::EPSILON)
				{
					if (math::lexicographical_less(lhs.texcoords, rhs.texcoords)) return true;
				}

				return false;
			}
		};

		typedef std::map<vertex_t, size_t, vertex_less> vertex_x_index_type;
		vertex_x_index_type vertex_x_index_;
	};

	class mesh_e: public sources_list_e {
	public:
		typedef std::set<std::pair<size_t, size_t> > position_index_to_vertex_index_type;

		void check() {
			if (!vertices_) throw std::logic_error("no <vertices> found");
			if (triangles_.empty()) throw std::logic_error("no <triangles> found");
		}

		void build_mesh(MeshPtr mesh_ptr);

		boost::ptr_vector<triangles_e> const &get_triangles() const
		{
			return triangles_;
		}

		position_index_to_vertex_index_type const &get_position_index_to_vertex_index_mapping()
		{
			return position_index_to_vertex_index_;
		}

	private:
		boost::scoped_ptr<vertices_e> vertices_;
		boost::ptr_vector<triangles_e> triangles_;

		vertex_indexer vertex_indexer_;
		position_index_to_vertex_index_type position_index_to_vertex_index_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child_ptr) {
			sources_list_e::process_child(child_name, child_id, child_ptr);

			if (child_name == "vertices")
			{
				vertices_.reset(construct<vertices_e>(child_ptr));
			}
			else if (child_name == "triangles")
			{
				triangles_.push_back(construct<triangles_e>(child_ptr));
			}
		}
	};

	class geometry_e: public element {
	private:
		std::string id_, name_;
		boost::scoped_ptr<mesh_e> mesh_ptr_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child_ptr) {
			if (child_name == "mesh") {
				mesh_ptr_.reset(construct<mesh_e>(child_ptr));
			}
		}

	public:
		std::string const &get_id() const { return id_; }
		std::string const &get_name() const { return name_; }

		void init(xercesc::DOMElement const *element_ptr)
		{
			id_ = xerces_get_attribute<std::string>(element_ptr, "id");
			name_ = xerces_get_attribute<std::string>(element_ptr, "name");
		}

		void check()
		{
			if (!mesh_ptr_) throw std::logic_error("<mesh> not found in <geometry>");
		}

		mesh_e *get_mesh() const
		{
			return mesh_ptr_.get();
		}
	};

	class library_geometries_e: public element {
	protected:
		boost::ptr_vector<geometry_e> geometries_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child_ptr) {
			if (child_name == "geometry")
			{
				geometries_.push_back(construct<geometry_e>(child_ptr));
			}
		}

	public:
		void build_meshes()
		{
			for (boost::ptr_vector<geometry_e>::iterator it = geometries_.begin(); it != geometries_.end(); it++)
			{
				LOG_DEBUG("building mesh id = " << it->get_id() << ", name = " << it->get_name());

				MeshPtr mesh_ptr = get_model()->add_mesh();

				mesh_ptr->id = it->get_id();
				mesh_ptr->name = it->get_name();

				it->get_mesh()->build_mesh(mesh_ptr);
			}
		}

		geometry_e const *get_geometry_by_id(std::string const &id)
		{
			for (boost::ptr_vector<geometry_e>::iterator it = geometries_.begin(); it != geometries_.end(); it++)
			{
				if (it->get_id() == id) return &*it;
			}

			throw std::logic_error("geometry id = " + id + " not found");
		}
	};

	class joints_e: public inputs_list_e {
	};

	class vertex_weights_e: public inputs_list_e {
	private:
		std::vector<size_t> vcount_;
		std::vector<size_t> v_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child_ptr) {
			inputs_list_e::process_child(child_name, child_id, child_ptr);

			if (child_name == "vcount")
			{
				parse_list(child_ptr, vcount_);
			}
			else if (child_name == "v")
			{
				parse_list(child_ptr, v_);
			}
		}

	public:
		std::vector<size_t> const &get_vcount() const { return vcount_; }
		std::vector<size_t> const &get_v() const { return v_; }
	};

	class skin_e: public sources_list_e {
	private:
		math::matrix<4,4> bind_shape_matrix_;
		boost::scoped_ptr<joints_e> joints_;
		boost::scoped_ptr<vertex_weights_e> vertex_weights_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child_ptr) {
			sources_list_e::process_child(child_name, child_id, child_ptr);

			if (child_name == "bind_shape_matrix") {
				parse_matrix(child_ptr, bind_shape_matrix_);
			}
			else if (child_name == "joints") {
				joints_.reset(construct<joints_e>(child_ptr));
			}
			else if (child_name == "vertex_weights") {
				vertex_weights_.reset(construct<vertex_weights_e>(child_ptr));
			}
		}

	public:
		void build_inverted_bind_matrices(MeshPtr mesh_ptr)
		{
			mesh_ptr->bind_shape_matrix = bind_shape_matrix_;

            {
                math::matrix<4,4> t;
                t.identity();
                mesh_ptr->inverted_bind_matrices.resize(get_model()->joints.size(), t);
            }

			source_e const *joints_source = get_source(joints_->get_input("JOINT")->get_source().substr(1));
			source_e const *inverted_bind_matrices_source = get_source(joints_->get_input("INV_BIND_MATRIX")->get_source().substr(1));

			for (size_t i = 0, n = joints_source->get_size(); i < n; i++)
			{
				std::string joint_name;
				math::matrix<4,4> inv_bind_matrix;

				joints_source->get(i, joint_name);
				inverted_bind_matrices_source->get(i, inv_bind_matrix);

				size_t joint_index = mesh_ptr->get_model()->get_joint_by_name(joint_name)->index;
				mesh_ptr->inverted_bind_matrices[joint_index] = inv_bind_matrix;
			}
		}

		void build_joint_weights(MeshPtr mesh_ptr)
		{
			mesh_e *mesh_element_ptr = get_root()->get_geometries()->get_geometry_by_id(mesh_ptr->id)->get_mesh();
			mesh_e::position_index_to_vertex_index_type const &position_index_to_vertex_index = mesh_element_ptr->get_position_index_to_vertex_index_mapping();

			input_e const *joints_input = vertex_weights_->get_input("JOINT");
			input_e const *weights_input = vertex_weights_->get_input("WEIGHT");

			source_e const *joints_source = get_source(joints_input->get_source().substr(1));
			source_e const *weights_source = get_source(weights_input->get_source().substr(1));

			size_t stride = 1 + std::max(joints_input->get_offset(), weights_input->get_offset());

			std::vector<size_t> const &vcount = vertex_weights_->get_vcount();
			std::vector<size_t> const &v = vertex_weights_->get_v();

			std::vector<size_t>::const_iterator v_get = v.begin();

			std::vector<math::vec<8, unsigned> > joint_indices_buffer;
			std::vector<math::vec<8> > joint_weights_buffer;

			joint_indices_buffer.resize(mesh_ptr->get_vertices_count());
			joint_weights_buffer.resize(mesh_ptr->get_vertices_count());

			size_t position_index = 0;

			mesh_ptr->joint_index_transform.resize(joints_source->get_size());

			for (size_t i = 0; i < joints_source->get_size(); i++)
			{
				std::string joint_name;
				joints_source->get(i, joint_name);
				mesh_ptr->joint_index_transform[i] = mesh_ptr->get_model()->get_joint_by_name(joint_name)->index;
			}

			for (std::vector<size_t>::const_iterator it = vcount.begin(); it != vcount.end(); it++)
			{
				math::vec<8, unsigned> joint_indices;
				math::vec<8> joint_weights;

                joint_indices.set_all(0);
                joint_weights.set_all(0);

				for (size_t i = 0, j = 0; i < *it; i++)
				{
                    unsigned index = 0;
                    math::scalar weight = 0;

					for (size_t offset = 0; offset < stride; offset++)
					{
						if (offset == joints_input->get_offset())
						{
							index = *v_get;
						}

						if (offset == weights_input->get_offset())
						{
							weights_source->get(*v_get, weight);
						}

						v_get++;
					}

                    if (weight > math::EPSILON)
                    {
                        if (j == 8)
                        {
							size_t ligtest_index = size_t(-1);
							float ligtest_weight = weight;

							for (size_t k = 0; k < 8; k++)
							{
								if (joint_weights.i[k] < ligtest_weight)
								{
									ligtest_index = k;
									ligtest_weight = joint_weights.i[k];
								}
							}

							if (ligtest_index == size_t(-1))
							{
								LOG_WARNING("vertex with more than 8 joint weights found in mesh " << mesh_ptr->id
									<< " and last one is lightest with weight = " << weight
									<< ", index = " << index << " -- ignoring it");
							}
							else
							{
								LOG_WARNING("vertex with more than 8 joint weights found in mesh " << mesh_ptr->id
									<< " and lightest weight index = " << ligtest_index
									<< ", weight = " << ligtest_weight
									<< ", joint = " << joint_indices.i[ligtest_index]
									<< " will be replaced with joint = " << index
									<< ", weight = " << weight);

								joint_indices.i[ligtest_index] = index;
								joint_weights.i[ligtest_index] = weight;
							}
                        }
						else
						{
	                        joint_indices.i[j] = index;
		                    joint_weights.i[j] = weight;

			                j++;
						}
                    }
				}

				// for all vertices coresponding to this position
				mesh_e::position_index_to_vertex_index_type::const_iterator it =
					position_index_to_vertex_index.lower_bound(std::make_pair(position_index, 0));

				while (it != position_index_to_vertex_index.end() && it->first == position_index)
				{
					joint_indices_buffer[it->second] = joint_indices;
					joint_weights_buffer[it->second] = joint_weights;

					it++;
				}

				position_index++;
			}

			mesh_ptr->lock();
			mesh_ptr->set_joint_indices(&joint_indices_buffer[0]);
			mesh_ptr->set_joint_weights(&joint_weights_buffer[0]);
			mesh_ptr->unlock();
		}
	};

	class controller_e: public element {
	private:
		boost::ptr_map<std::string, skin_e *> skins_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child_ptr)
		{
			skins_[xerces_get_attribute<std::string>(child_ptr, "source")] = construct<skin_e>(child_ptr);
		}

	public:
		std::string get_skin_source_id() const
		{
			if (skins_.size() != 1) throw std::logic_error("zero or more than one <skin> in <controller>");
			return skins_.begin()->first.substr(1);
		}

		void build_joint_influences() {
			for (boost::ptr_map<std::string, skin_e *>::const_iterator it = skins_.begin(); it != skins_.end(); it++) {
				MeshPtr mesh_ptr = get_model()->get_mesh_by_id(it->first.substr(1));
				(*it->second)->build_inverted_bind_matrices(mesh_ptr);
				(*it->second)->build_joint_weights(mesh_ptr);
			}
		}
	};

	class library_controllers_e: public element {
	private:
		boost::ptr_map<std::string, controller_e *> controllers_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child_ptr) {
			if (child_name == "controller")
			{
				controllers_[child_id] = construct<controller_e>(child_ptr);
			}
		}

	public:
		controller_e const *get_controller_by_id(std::string const &id) const
		{
			boost::ptr_map<std::string, controller_e *>::const_iterator it = controllers_.find(id);
			if (it != controllers_.end()) return *it->second;

			throw std::logic_error("controller with id = " + id + " not found");
		}

		void build_joint_influences() {
			for (boost::ptr_map<std::string, controller_e *>::const_iterator it = controllers_.begin(); it != controllers_.end(); it++) {
				(*it->second)->build_joint_influences();
			}
		}
	};

	class sampler_e: public inputs_list_e {
	private:
		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child_ptr) {
			inputs_list_e::process_child(child_name, child_id, child_ptr);
		}
	};

	class animation_e: public sources_list_e {
	private:
		struct channel {
			std::string source_url;
			std::string target_url;

			channel(std::string const &s, std::string const &t):
			source_url(s),
				target_url(t)
			{
			}
		};

		std::vector<channel> channels_;
		boost::ptr_map<std::string, sampler_e *> samplers_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child_ptr)
		{
			sources_list_e::process_child(child_name, child_id, child_ptr);

			if (child_name == "sampler")
			{
				samplers_[child_id] = construct<sampler_e>(child_ptr);
			}
			else if (child_name == "channel")
			{
				channels_.push_back(channel(xerces_get_attribute<std::string>(child_ptr, "source"), xerces_get_attribute<std::string>(child_ptr, "target")));
			}
		}

	public:
		Animation build_animation(Model *model_ptr)
		{
			Animation result;

			for (std::vector<channel>::const_iterator it = channels_.begin(); it != channels_.end(); it++) {
				boost::ptr_map<std::string, sampler_e *>::iterator jt = samplers_.find(it->source_url.substr(1));
				if (jt == samplers_.end()) throw std::logic_error("sampler for url " + it->source_url + " not found");

				sampler_e *sampler_ptr = *jt->second;

				source_e const *time_input = get_source(sampler_ptr->get_input("INPUT")->get_source().substr(1));
				source_e const *transform_output = get_source(sampler_ptr->get_input("OUTPUT")->get_source().substr(1));

				Sampler dummy;
				transform_e const *transform_ptr = 0;

				// target is in form <node>/<transform>.<param>, <param> may be omitted
				std::vector<std::string> parsed_target_components;

				{
					using namespace boost::lambda;
					using boost::lambda::_1;
					boost::split(parsed_target_components, it->target_url, _1 == '/' || _1 == '.', boost::algorithm::token_compress_on);
				}

				if (parsed_target_components.size() == 2) parsed_target_components.push_back(std::string());
				if (parsed_target_components.size() != 3) throw std::logic_error("invalid url " + it->target_url);

				JointPtr joint_ptr = model_ptr->get_joint_by_id(parsed_target_components[0]);
				dummy.set_target(joint_ptr, parsed_target_components[1]);

				transform_ptr = get_root()->get_visual_scenes()->get_node_by_id(parsed_target_components[0])->get_transform_by_sid(parsed_target_components[1]);

				for (size_t i = 0, n = time_input->get_size(); i < n; i++) {
					float t;
					time_input->get(i, t);

					math::matrix<4,4> m = transform_ptr->get_baked_sample(transform_output, i, parsed_target_components.back());

					dummy.add_sample(t, m);
				}

				result.samplers.push_back(dummy);
			}

			return result;
		}
	};

	class library_animations_e: public element {
	private:
		boost::ptr_map<std::string, animation_e *> animations_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child_ptr) {
			if (child_name == "animation") {
				animations_[child_id] = construct<animation_e>(child_ptr);
			}
		}

	public:
		void build_animations() {
			for (boost::ptr_map<std::string, animation_e *>::iterator it = animations_.begin(); it != animations_.end(); it++) {
				get_model()->animations.push_back(AnimationPtr(new Animation((*it->second)->build_animation(get_model()))));
				get_model()->animations.back()->name = it->first;
			}
		}
	};

	class material_e: public element {
	private:
		std::string id_, name_;
		std::string instance_effect_url_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child_ptr)
		{
			if (child_name == "instance_effect")
			{
				instance_effect_url_ = xerces_get_attribute<std::string>(child_ptr, "url");
			}
		}

	public:
		void init(xercesc::DOMElement const *element_ptr)
		{
			id_ = xerces_get_attribute<std::string>(element_ptr, "id");
			name_ = xerces_get_attribute<std::string>(element_ptr, "name");
		}

		void check()
		{
			if (instance_effect_url_.empty()) throw std::logic_error("no <instance_effect> url specified for material");
		}

		MaterialPtr build_material() const;
	};

	class library_materials_e: public element {
	private:
		boost::ptr_map<std::string, material_e *> materials_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child_ptr)
		{
			if (child_name == "material")
			{
				materials_[child_id] = construct<material_e>(child_ptr);
			}
		}

	public:
		material_e const *get_material_by_name(std::string const &name) const
		{
			boost::ptr_map<std::string, material_e *>::const_iterator it = materials_.find(name);
			if (it != materials_.end()) return *it->second;

			throw std::logic_error("material " + name + " not found");
		}

		void library_materials_e::build_materials() const
		{
			LOG_DEBUG("building materials...");

			for (boost::ptr_map<std::string, material_e *>::const_iterator it = materials_.begin();
				it != materials_.end(); it++)
			{
				get_model()->materials.push_back((*it->second)->build_material());
			}
		}
	};

	class surface_e: public element {
	private:
		std::string init_from_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child_ptr)
		{
			if (child_name == "init_from")
			{
				init_from_ = xerces_get_text(child_ptr);
			}
		}

	public:
		std::string const &get_init_from() const
		{
			return init_from_;
		}
	};

	class sampler2d_e: public element {
	private:
		std::string source_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child_ptr)
		{
			if (child_name == "source")
			{
				source_ = xerces_get_text(child_ptr);
			}
		}

	public:
		std::string const &get_source() const
		{
			return source_;
		}
	};

	class newparam_e: public element {
	private:
		boost::scoped_ptr<surface_e> surface_ptr_;
		boost::scoped_ptr<sampler2d_e> sampler2d_ptr_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child_ptr)
		{
			if (child_name == "surface")
			{
				surface_ptr_.reset(construct<surface_e>(child_ptr));
			}
			else if (child_name == "sampler2D")
			{
				sampler2d_ptr_.reset(construct<sampler2d_e>(child_ptr));
			}
		}

	public:
		surface_e const *get_surface() const
		{
			return surface_ptr_.get();
		}

		sampler2d_e const *get_sampler() const
		{
			return sampler2d_ptr_.get();
		}

		void check()
		{
			if (surface_ptr_ && sampler2d_ptr_)
			{
				throw std::logic_error("only one of <surface> or <sampler2D> may be defined in <newparam>");
			}
		}
	};

	class phong_lambert_blinn_e: public element {
	private:
		std::map<std::string, std::string> textures_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child_ptr)
		{
			xercesc::DOMNodeList *textures_list_ptr = child_ptr->getElementsByTagName(xerces_lcp_transcode("texture").c_str());
			if (textures_list_ptr->getLength() == 0) return;
			if (textures_list_ptr->getLength() > 1) throw std::logic_error("more than one <texture> elements in effect");
			textures_.insert(std::make_pair(child_name, xerces_get_attribute<std::string>(textures_list_ptr->item(0), "texture")));
		}

	public:
		std::string get_sampler_name(std::string const &component_name) const
		{
			std::map<std::string, std::string>::const_iterator it = textures_.find(component_name);
			if (it != textures_.end()) return it->second;

			return std::string();
		}
	};

	class technique_e: public element {
	private:
		boost::scoped_ptr<phong_lambert_blinn_e> shader_ptr_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child_ptr)
		{
			if (child_name == "phong" || child_name == "lambert" || child_name == "blinn")
			{
				if (shader_ptr_) throw std::logic_error("shader for <technique> defined more than one time");

				shader_ptr_.reset(construct<phong_lambert_blinn_e>(child_ptr));
			}
		}

	public:
		void check()
		{
			if (!shader_ptr_) throw std::logic_error("material not found in <technique> or unknown material type");
		}

		phong_lambert_blinn_e const *get_shader() const
		{
			return shader_ptr_.get();
		}
	};

	class profile_common_e: public element {
	private:
		boost::ptr_map<std::string, newparam_e *> newparams_;
		boost::scoped_ptr<technique_e> technique_ptr_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child_ptr)
		{
			if (child_name == "newparam")
			{
				std::string const &sid = xerces_get_attribute<std::string>(child_ptr, "sid");
				newparams_[sid] = construct<newparam_e>(child_ptr);
			}
			else if (child_name == "technique")
			{
				if (technique_ptr_) throw std::logic_error("<technique> in <profile_common> defined more than one time");

				technique_ptr_.reset(construct<technique_e>(child_ptr));
			}
		}

		newparam_e const *get_newparam(std::string const &name) const
		{
			boost::ptr_map<std::string, newparam_e *>::const_iterator it = newparams_.find(name);
			if (it == newparams_.end()) throw std::logic_error("<newparam> with name " + name + " not found");

			return *it->second;
		}

		std::string get_surface_filename(std::string const &component_name) const;

	public:
		MaterialPtr build_material() const
		{
			MaterialPtr material_ptr(new Material(get_model()->get_renderer()));

			std::string const &diffuse_texture = get_surface_filename("diffuse");

			if (diffuse_texture.size())
			{
				std::string const &filename = (get_root()->get_basepath() / diffuse_texture).native_file_string();

				LOG_DEBUG("texture = " << diffuse_texture << ", filename = " << filename);
				material_ptr->set_texture(filename);
			}
			else
			{
				LOG_DEBUG("no texture");
			}

			// NOT IMPLEMENTED
			/*
			std::string const &specular_texture = get_surface_filename("specular");

			if (specular_texture.size())
			{
				LOG_DEBUG("specular_texture = " << diffuse_texture);
				material_ptr->set_specular_map(specular_texture);
			}
			else
			{
				LOG_DEBUG("no specular_texture");
			}
			*/

			return material_ptr;
		}
	};

	class effect_e: public element {
	private:
		boost::scoped_ptr<profile_common_e> profile_common_ptr_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child_ptr)
		{
			if (child_name == "profile_COMMON")
			{
				profile_common_ptr_.reset(construct<profile_common_e>(child_ptr));
			}
		}

	public:
		MaterialPtr build_material() const
		{
			return profile_common_ptr_->build_material();
		}
	};

	class library_effects_e: public element {
	private:
		boost::ptr_map<std::string, effect_e *> effects_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child_ptr)
		{
			if (child_name == "effect")
			{
				effects_[child_id] = construct<effect_e>(child_ptr);
			}
		}

	public:
		effect_e const *get_effect_by_name(std::string const &name) const
		{
			boost::ptr_map<std::string, effect_e *>::const_iterator it = effects_.find(name);
			if (it != effects_.end()) return *it->second;

			throw std::logic_error("effect " + name + " not found");
		}
	};

	class image_e: public element {
	private:
		std::string init_from_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child_ptr)
		{
			if (child_name == "init_from")
			{
				init_from_ = xerces_get_text(child_ptr);
			}
		}

	public:
		std::string const &get_init_from() const
		{
			return init_from_;
		}
	};

	class library_images_e: public element {
	private:
		boost::ptr_map<std::string, image_e *> images_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child_ptr)
		{
			if (child_name == "image")
			{
				images_[child_id] = construct<image_e>(child_ptr);
			}
		}

	public:
		image_e const *get_image_by_name(std::string const &name)
		{
			boost::ptr_map<std::string, image_e *>::const_iterator it = images_.find(name);
			if (it != images_.end()) return *it->second;

			throw std::logic_error("image " + name + " not found");
		}
	};

	void bind_material_e::bind_mesh_materials(mesh_e const *mesh_element_ptr, MeshPtr mesh_ptr) const
	{
		boost::ptr_vector<triangles_e> const &triangles = mesh_element_ptr->get_triangles();

		Mesh::triangles_list_type::iterator mesh_triangles_it = mesh_ptr->triangles_list.begin();
		boost::ptr_vector<triangles_e>::const_iterator triangles_it = triangles.begin();

		while (mesh_triangles_it != mesh_ptr->triangles_list.end() && triangles_it != triangles.end())
		{
			std::string const &material_symbol = triangles_it->get_material();
			std::string const &material_name = get_technique_common()->get_material_by_symbol(material_symbol);

			LOG_DEBUG("material_symbol = " << material_symbol << ", material_name = " << material_name);

			mesh_triangles_it->material_ptr = mesh_ptr->get_model()->get_material_by_name(material_name);

			mesh_triangles_it++;
			triangles_it++;
		}

		if (mesh_triangles_it != mesh_ptr->triangles_list.end() || triangles_it != triangles.end())
		{
			throw std::logic_error("mismatching number of <triangles> and triangles_lists in mesh with id = " + mesh_ptr->id);
		}
	}

	void mesh_e::build_mesh(MeshPtr mesh_ptr)
	{
		input_e const *position_input = vertices_->get_input("POSITION");
		source_e const *position_source = get_source(position_input->get_source().substr(1));

		std::vector<math::vec<3> > positions_buffer;
		std::vector<math::vec<3> > normals_buffer;
		std::vector<math::vec<2> > texcoords_buffer;
		std::vector<unsigned> index_buffer;

		size_t first_index_offset = 0;

		for (boost::ptr_vector<triangles_e>::const_iterator it = triangles_.begin();
			it != triangles_.end(); it++)
		{
			triangles_e const *triangles_ptr = &*it;

			input_e const *normals_input = 0;
			try	{ normals_input = vertices_->get_input("NORMAL"); }
			catch(...) { normals_input = triangles_ptr->get_input("NORMAL"); }

			source_e const *normals_source = get_source(normals_input->get_source().substr(1));

			input_e const *texcoords_input = triangles_ptr->get_input("TEXCOORD");
			source_e const *texcoords_source = get_source(texcoords_input->get_source().substr(1));

			std::vector<size_t> const &indices = triangles_ptr->get_indices();
			std::vector<size_t>::const_iterator indices_get = indices.begin();

			size_t stride = triangles_ptr->get_stride();

			Mesh::triangles dummy;
			dummy.triangles_count = triangles_ptr->get_count();
			dummy.first_index_offset = first_index_offset;
			dummy.minimal_vertex_index = size_t(-1);
			dummy.vertices_count = 0;

			for (size_t i = 0, n = triangles_ptr->get_count() * 3; i < n; i++) {
				size_t position_index;
				vertex_indexer::vertex_t v;

				for (size_t j = 0; j < stride; j++) {
					if (j == position_input->get_offset()) {
						position_index = *indices_get;
						position_source->get(position_index, v.position);
					}

					if (j == normals_input->get_offset()) {
						normals_source->get(*indices_get, v.normal);
					}

					if (j == texcoords_input->get_offset()) {
						texcoords_source->get(*indices_get, v.texcoords);
					}

					indices_get++;
				}

				std::pair<size_t, bool> vertex_index = vertex_indexer_.choose_index(v);

				if (vertex_index.second)
				{
					position_index_to_vertex_index_.insert(std::make_pair(position_index, vertex_index.first));

					positions_buffer.push_back(v.position);
					normals_buffer.push_back(v.normal);
					texcoords_buffer.push_back(v.texcoords);

					dummy.vertices_count++;
				}

				if (vertex_index.first < dummy.minimal_vertex_index)
				{
					dummy.minimal_vertex_index = vertex_index.first;
				}

				index_buffer.push_back(vertex_index.first);
			}

			mesh_ptr->triangles_list.push_back(dummy);

			first_index_offset += triangles_ptr->get_count() * 3;
		}

		mesh_ptr->set_vertices_count(positions_buffer.size());
		mesh_ptr->set_indices_count(index_buffer.size());
		mesh_ptr->lock();
		mesh_ptr->set_positions(&positions_buffer[0]);
		mesh_ptr->set_normals(&normals_buffer[0]);
		mesh_ptr->set_texcoords(&texcoords_buffer[0]);
		mesh_ptr->set_face_indices(&index_buffer[0]);
		mesh_ptr->unlock();
	}

	void node_e::build_hierarchy(JointPtr parent_ptr)
	{
		JointPtr joint_ptr = get_model()->add_joint();
		joint_ptr->id = id_;
		joint_ptr->name = sid_;
		joint_ptr->parent = parent_ptr;

		for (boost::ptr_vector<transform_e>::const_iterator it = transforms_.begin(); it != transforms_.end(); it++) {
			joint_ptr->transforms.push_back(Joint::transform(it->get_sid(), it->get_baked_matrix()));
		}

		if (parent_ptr) parent_ptr->childs.push_back(joint_ptr);

		if (instance_geometry_)
		{
			MeshPtr mesh_ptr = get_model()->get_mesh_by_id(instance_geometry_->get_url());

			joint_ptr->mesh_ptr = mesh_ptr;
			mesh_e const *mesh_element_ptr = get_root()->get_geometries()->get_geometry_by_id(instance_geometry_->get_url())->get_mesh();

			instance_geometry_->get_bind_material()->bind_mesh_materials(mesh_element_ptr, mesh_ptr);
		}

		if (instance_controller_)
		{
			controller_e const *controller_ptr = get_root()->get_controllers()->get_controller_by_id(instance_controller_->get_url());

			joint_ptr->mesh_ptr = get_model()->get_mesh_by_id(controller_ptr->get_skin_source_id());
			mesh_e const *mesh_element_ptr = get_root()->get_geometries()->get_geometry_by_id(controller_ptr->get_skin_source_id())->get_mesh();

			instance_controller_->get_bind_material()->bind_mesh_materials(mesh_element_ptr, joint_ptr->mesh_ptr.lock());
		}

		for (boost::ptr_vector<node_e>::iterator it = childs_.begin(); it != childs_.end(); it++)
		{
			it->build_hierarchy(joint_ptr);
		}
	}

	MaterialPtr material_e::build_material() const
	{
		LOG_DEBUG("material " << name_);

		MaterialPtr material_ptr(get_root()->
			get_effects()->
			get_effect_by_name(instance_effect_url_.substr(1))->
			build_material());

		material_ptr->set_name(name_);

		return material_ptr;
	}

	std::string profile_common_e::get_surface_filename(std::string const &component_name) const
	{
		std::string const &sampler_name = technique_ptr_->get_shader()->get_sampler_name(component_name);
		if (sampler_name.empty()) return sampler_name;

		newparam_e const *sampler_ptr = get_newparam(sampler_name);

		std::string const &surface_name = sampler_ptr->get_sampler()->get_source();
		newparam_e const *surface_ptr = get_newparam(surface_name);

		std::string const &image_name = surface_ptr->get_surface()->get_init_from();

		std::string const &filepath = get_root()->get_images()->get_image_by_name(image_name)->get_init_from();

		return boost::filesystem::path(filepath).filename();
	}

	class collada_e: public element, public root_element {
	private:
		Model *current_model_ptr_;
		boost::scoped_ptr<library_visual_scenes_e> visual_scenes_;
		boost::scoped_ptr<library_geometries_e> geometries_;
		boost::scoped_ptr<library_controllers_e> controllers_;
		boost::scoped_ptr<library_animations_e> animations_;
		boost::scoped_ptr<library_materials_e> materials_;
		boost::scoped_ptr<library_effects_e> effects_;
		boost::scoped_ptr<library_images_e> images_;
		boost::filesystem::path basepath_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child_ptr)
		{
			if (child_name == "library_visual_scenes")
			{
				visual_scenes_.reset(construct<library_visual_scenes_e>(child_ptr));
			}
			else if (child_name == "library_geometries")
			{
				geometries_.reset(construct<library_geometries_e>(child_ptr));
			}
			else if (child_name == "library_controllers")
			{
				controllers_.reset(construct<library_controllers_e>(child_ptr));
			}
			else if (child_name == "library_animations")
			{
				animations_.reset(construct<library_animations_e>(child_ptr));
			}
			else if (child_name == "library_materials")
			{
				materials_.reset(construct<library_materials_e>(child_ptr));
			}
			else if (child_name == "library_effects")
			{
				effects_.reset(construct<library_effects_e>(child_ptr));
			}
			else if (child_name == "library_images")
			{
				images_.reset(construct<library_images_e>(child_ptr));
			}
		}

	public:
		collada_e():
			current_model_ptr_(0)
		{
			set_root(this);
		}

		Model *get_model() const
		{
			return current_model_ptr_;
		}

		library_visual_scenes_e *get_visual_scenes() const
		{
			return visual_scenes_.get();
		}

		library_geometries_e *get_geometries() const
		{
			return geometries_.get();
		}

		library_controllers_e *get_controllers() const
		{
			return controllers_.get();
		}

		library_materials_e *get_materials() const
		{
			return materials_.get();
		}

		library_effects_e *get_effects() const
		{
			return effects_.get();
		}

		library_images_e *get_images() const
		{
			return images_.get();
		}

		boost::filesystem::path const &get_basepath() const
		{
			return basepath_;
		}

		void check()
		{
			if (!visual_scenes_) throw std::logic_error("<library_visual_scenes> not found");
			if (!geometries_) throw std::logic_error("<library_geometries> not found");
		}

		void set_filename(std::string const &filename)
		{
			basepath_ = boost::filesystem::path(filename, boost::filesystem::native).parent_path();
		}

		void build_model(Model *model_ptr)
		{
			current_model_ptr_ = model_ptr;

			try
			{
				geometries_->build_meshes();
				if (materials_) materials_->build_materials();
				visual_scenes_->build_hierarchy();
				if (controllers_) controllers_->build_joint_influences();
				if (animations_) animations_->build_animations();
			}
			catch(...)
			{
				current_model_ptr_ = 0;
				throw;
			}

			current_model_ptr_ = 0;
		}
	};
}

namespace graphic
{

class ColladaLoader::Impl
{
public:
	collada_elements::collada_e *get_root()
	{
		return &root_;
	}

private:
	collada_elements::collada_e root_;
};

ColladaLoader::ColladaLoader(Model *model_ptr):
model_ptr_(model_ptr),
impl_(new Impl())
{
}

ColladaLoader::~ColladaLoader()
{
}

void ColladaLoader::load(std::string const &filename)
{
	try
	{
		LOG_INFO("parsing collada file " << filename);

		boost::scoped_ptr<xercesc::XercesDOMParser> parser(new xercesc::XercesDOMParser());
		parser->setValidationScheme(xercesc::XercesDOMParser::Val_Always);
		parser->setDoNamespaces(true);

		boost::scoped_ptr<xercesc::ErrorHandler> error_handler(new xercesc::HandlerBase());
		parser->setErrorHandler(error_handler.get());

		parser->parse(filename.c_str());

		xercesc::DOMDocument *document = parser->getDocument();

		xercesc::DOMNodeList *node_list = document->getChildNodes();
		assert (node_list->getLength() == 1);

		xercesc::DOMNode *root_node = node_list->item(0);
		if (util::xerces_lcp_transcode(root_node->getLocalName()) != "COLLADA")
			throw load_error("root element is not <COLLADA />");

		impl_->get_root()->set_filename(filename);
		impl_->get_root()->go((xercesc::DOMElement *) root_node);

		LOG_INFO("finished parsing " << filename);
	}
	catch (std::exception const &error)
	{
		throw load_error("error loading collada " + filename + ": " + error.what());
	}
}

void ColladaLoader::build_model()
{
	impl_->get_root()->build_model(model_ptr_);
}

}

