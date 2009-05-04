
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
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <math/matrix.h>
#include <util/xerces_lcp_transcoder.h>
#include <graphic/collada_loader.h>

#if defined(max)
#undef max
#endif

namespace collada_elements {
	using namespace engine::util;
	using namespace engine::graphic;

	typedef model::joint_influence::vertex_weight vertex_weight;

	class library_visual_scenes;
	class library_controllers;

	class root_element {
	public:
		virtual library_visual_scenes *get_visual_scenes() = 0;
		virtual library_controllers *get_controllers() = 0;
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
		void set_root(root_element *root) { root_ = root; }

		void init(xercesc::DOMElement const *element_ptr) {}
		virtual void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) {}
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
		parse_list(xercesc::DOMElement const *element_ptr, std::vector<T> &v) {
			std::stringstream ss(xerces_get_text(element_ptr));
			T dummy;
			while (ss >> dummy) v.push_back(dummy);
			if (ss.bad()) throw std::runtime_error("can't parse list");
		}

		template<class T> static size_t
		parse_list(xercesc::DOMElement const *element_ptr, T *list_ptr, size_t size) {
			std::stringstream ss(xerces_get_text(element_ptr));
			size_t i = 0;
			while (i < size && ss >> *list_ptr++) i++;
			return i;
		}

		void parse_matrix(xercesc::DOMElement const *element_ptr, math::matrix<4, 4> &tf) {
			if (parse_list(element_ptr, &tf.ij[0][0], 16) != 16) throw std::runtime_error("invalid <matrix>");
			tf.transpose_inplace();
		}
	};

	class technique_common: public element {
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

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) {
			if (child_name == "accessor") {
				count_ = xerces_get_attribute<size_t>(child, "count");
				stride_ = xerces_get_attribute<size_t>(child, "stride", 3);

				xercesc::DOMNodeList const *params = child->getElementsByTagName(xerces_lcp_transcode("param").c_str());
				for (size_t i = 0, n = params->getLength(); i < n; i++) {
					xercesc::DOMNode const *node = params->item(i);
					params_.push_back(param(xerces_get_attribute<std::string>(node, "name"), xerces_get_attribute<std::string>(node, "type")));
				}
			}
		}

	public:
		size_t get_stride() const { return stride_; }
		size_t get_count() const { return count_; }
	};

	class source: public element {
	private:
		std::vector<float> float_array_;
		std::vector<std::string> name_array_;
		boost::scoped_ptr<technique_common> technique_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) {
			if (child_name == "float_array") {
				parse_list(child, float_array_);
			}
			else if (child_name == "Name_array") {
				parse_list(child, name_array_);
			}
			else if (child_name == "technique_common") {
				technique_.reset(construct<technique_common>(child));
			}
		}

	public:
		technique_common const &get_technique() const { return *technique_; }

		void check() {
			if (float_array_.empty() && name_array_.empty()) throw std::runtime_error("malformed or unknown <source>");
			if (!technique_) throw std::runtime_error("no <technique_common> in <source>");

			size_t array_size = std::max(float_array_.size(), name_array_.size());
			size_t expected_size = technique_->get_stride() * technique_->get_count();

			if (array_size != expected_size)
			{
				throw std::runtime_error("array has " + boost::lexical_cast<std::string>(expected_size) 
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

	class sources_list: public element {
	protected:
		boost::ptr_map<std::string, source> sources_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) {
			if (child_name == "source") {
				sources_.insert(const_cast<std::string &>(child_id), construct<source>(child));
			}
		}

	public:
		source const *get_source(std::string const &id) const {
			boost::ptr_map<std::string, source>::const_iterator it = sources_.find(id);
			if (it == sources_.end()) throw std::runtime_error("source id = " + id + " not found");
			return it->second;
		}
	};

	class transform: public element {
	protected:
		std::string sid_;

	public:
		void init(xercesc::DOMElement const *element_ptr)
		{
			sid_ = xerces_get_attribute<std::string>(element_ptr, "sid");
		}

		std::string const &get_sid() const { return sid_; }

		virtual math::matrix<4,4> get_baked_matrix() const = 0;
		virtual math::matrix<4,4> get_baked_sample(source const *source_ptr, int index, std::string const &param) const = 0;
	};

	class identity_transform: public transform {
	public:
		math::matrix<4,4> get_baked_matrix() const
	    {
			math::matrix<4,4> m;
			m.identity();
			return m;
		}

		math::matrix<4,4> get_baked_sample(source const *source_ptr, int index, std::string const &param) const
		{
			throw std::logic_error("can't read identity transform sample from source");
	    }
	};


	class matrix_transform: public transform {
	private:
		math::matrix<4,4> matrix_;

	public:
		void init(xercesc::DOMElement const *element_ptr)
		{
			transform::init(element_ptr);
			parse_matrix(element_ptr, matrix_);
		}

		math::matrix<4,4> get_baked_matrix() const
		{
			return matrix_;
		}

		math::matrix<4,4> get_baked_sample(source const *source_ptr, int index, std::string const &param) const
		{
			math::matrix<4,4> result;
			source_ptr->get(index, result);
			return result;
		}
	};

	class translation_transform: public transform {
	private:
		math::vec<3> translate_;

	public:
		void init(xercesc::DOMElement const *element_ptr)
		{
			transform::init(element_ptr);
			parse_list(element_ptr, translate_.i, 3);
		}

	    math::matrix<4,4> get_baked_matrix() const
		{
			math::matrix<4,4> result;
			result.translation(translate_);
			return result;
		}

		math::matrix<4,4> get_baked_sample(source const *source_ptr, int index, std::string const &param) const
		{
			math::vec<3> translate;
			source_ptr->get(index, translate);

			math::matrix<4,4> result;
			result.translation(translate);

			return result;
		}
	};

	class rotation_transform: public transform {
	private:
		math::vec<3> axis_;
		math::scalar angle_;

	public:
		void init(xercesc::DOMElement const *element_ptr)
		{
			transform::init(element_ptr);

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

		math::matrix<4,4> get_baked_sample(source const *source_ptr, int index, std::string const &param) const
		{
			if (param != "ANGLE") throw std::runtime_error("unexpected rotation transform param type " + param);

			math::scalar angle;
			source_ptr->get(index, angle);

			math::matrix<4,4> result;
			result.rotation(axis_, angle * math::PI / 180.0f);

			return result;
		}
	};

	class scale_transform: public transform {
	private:
		math::vec<3> scale_;

	public:
		void init(xercesc::DOMElement const *element_ptr)
		{
			transform::init(element_ptr);
			parse_list(element_ptr, scale_.i, 3);
		}

		math::matrix<4,4> get_baked_matrix() const
		{
			math::matrix<4,4> result;
			result.scaling(scale_);
			return result;
		}

		math::matrix<4,4> get_baked_sample(source const *source_ptr, int index, std::string const &param) const
		{
			math::vec<3> scale;
			source_ptr->get(index, scale);

			math::matrix<4,4> result;
			result.scaling(scale);

			return result;
		}
	};

	class instance_controller: public element {
	private:
		std::string url_;
		std::string skeleton_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child)
		{
			if (child_name == "skeleton")
			{
				skeleton_ = xerces_get_text(child).substr(1);
			}
		}

	public:
		std::string const &get_url() const { return url_; }
		std::string const &get_skeleton() const { return skeleton_; }

		void init(xercesc::DOMElement const *element_ptr)
		{
			url_ = xerces_get_attribute<std::string>(element_ptr, "url").substr(1);
		}
	};

	class node: public element {
	protected:
		std::string id_, sid_, name_, type_, instance_geometry_;
		node *parent_;
		boost::ptr_vector<node> childs_;
		boost::ptr_vector<transform> transforms_;
		boost::scoped_ptr<instance_controller> instance_controller_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child)
		{
			if (child_name == "node")
			{
				childs_.push_back(construct<node>(child));
				childs_.back().parent_ = this;
			}
			else if (child_name == "matrix")
			{
				transforms_.push_back(construct<matrix_transform>(child));
			}
			else if (child_name == "translate")
			{
				transforms_.push_back(construct<translation_transform>(child));
			}
			else if (child_name == "rotate")
			{
				transforms_.push_back(construct<rotation_transform>(child));
			}
			else if (child_name == "scale")
			{
				transforms_.push_back(construct<scale_transform>(child));
			}
			else if (child_name == "instance_geometry")
			{
				instance_geometry_ = xerces_get_attribute<std::string>(child, "url");
			}
			else if (child_name == "instance_controller")
			{
				instance_controller_.reset(construct<instance_controller>(child));
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
				transforms_.push_back(new identity_transform());
			}
		}

		void build_hierarchy(memory_model *model_ptr, model::joint *parent_ptr);

		node const *get_node_by_id(std::string const &id) const
		{
			if (id == id_) return this;

			for (boost::ptr_vector<node>::const_iterator it = childs_.begin(); it != childs_.end(); it++)
			{
				if (node const *node_ptr = it->get_node_by_id(id)) return node_ptr;
			}

			return 0;
		}

		transform const *get_transform_by_sid(std::string const &sid) const
		{
			for (boost::ptr_vector<transform>::const_iterator it = transforms_.begin();
				it != transforms_.end(); it ++)
			{
				if (it->get_sid() == sid) return &*it;
			}

			throw std::logic_error("transform with sid = " + sid + " not found in node id = " + id_);
		}
	};

	class visual_scene: public node {
	public:
		void check() {
		}

		void build_hierarchy(memory_model *model_ptr) {
			for (boost::ptr_vector<node>::iterator it = childs_.begin(); it != childs_.end(); it++) {
				it->build_hierarchy(model_ptr, 0);
			}
		}
	};

	class library_visual_scenes: public element {
	protected:
		boost::ptr_map<std::string, visual_scene> visual_scenes_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) {
			if (child_name == "visual_scene") {
				visual_scenes_.insert(const_cast<std::string &>(child_id), construct<visual_scene>(child));
			}
		}

	public:
		node const *get_node_by_id(std::string const &id) const
		{
			for (boost::ptr_map<std::string, visual_scene>::const_iterator it = visual_scenes_.begin();
				it != visual_scenes_.end(); it++)
			{
				if (node const *node_ptr = it->second->get_node_by_id(id)) return node_ptr;
			}

			throw std::logic_error("node with id = " + id + " not found");
		}

		void build_hierarchy(memory_model *model_ptr) {
			if (visual_scenes_.size() != 1) throw std::runtime_error("expecting one <visual_scene> in <library_visual_scenes>");
			visual_scenes_.begin()->second->build_hierarchy(model_ptr);
		}
	};

	class input: public element {
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

	class inputs_list: public element {
	protected:
		boost::ptr_map<std::string, input> inputs_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) {
			if (child_name == "input") {
				inputs_.insert(xerces_get_attribute<std::string>(child, "semantic"), construct<input>(child));
			}
		}

	public:
		input const *get_input(std::string const &semantic) const {
			boost::ptr_map<std::string, input>::const_iterator it = inputs_.find(semantic);
			if (it == inputs_.end()) throw std::runtime_error("input with semantic = " + semantic + " not found");
			return it->second;
		}
	};

	class vertices: public inputs_list {
	};

	class triangles: public inputs_list {
	private:
		size_t count_;
		std::vector<size_t> indices_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) {
			if (child_name == "p") {
				parse_list(child, indices_);
			}

			inputs_list::process_child(child_name, child_id, child);
		}

	public:
		void init(xercesc::DOMElement const *element_ptr) {
			count_ = xerces_get_attribute<size_t>(element_ptr, "count");
		}

		size_t get_count() const { return count_; }
		std::vector<size_t> const &get_indices() const { return indices_; }
	};

	class vertex_indexer {
	private:
		struct vertex_less {
			bool operator ()(memory_mesh::vertex const &lhs, memory_mesh::vertex const &rhs) {
				if ((lhs.position - rhs.position).length_sq() > math::EPSILON)
				{
					if (lhs.position <= rhs.position) return true;
					if (lhs.position >= rhs.position) return false;
				}

				if ((lhs.normal - rhs.normal).length_sq() > math::EPSILON)
				{
					if (lhs.normal <= rhs.normal) return true;
					if (lhs.normal >= rhs.normal) return false;
				}

				if ((lhs.texcoords - rhs.texcoords).length_sq() > math::EPSILON)
				{
					return lhs.texcoords <= rhs.texcoords;
				}

				return false;
			}
		};

		typedef std::map<memory_mesh::vertex, size_t, vertex_less> vertex_x_index_type;
		vertex_x_index_type vertex_x_index_;

	public:
		std::pair<size_t, bool> choose_index(memory_mesh::vertex const &v) {
			vertex_x_index_type::const_iterator it = vertex_x_index_.find(v);
			if (it != vertex_x_index_.end()) return std::pair<size_t, bool>(it->second, false);

			size_t index = vertex_x_index_.size();
			vertex_x_index_.insert(std::make_pair(v, index));
			return std::pair<size_t, bool>(index, true);
		}
	};

	class mesh: public sources_list {
	private:
		boost::scoped_ptr<vertices> vertices_;
		boost::scoped_ptr<triangles> triangles_;

		vertex_indexer vertex_indexer_;

		typedef std::set<std::pair<size_t, size_t> > position_index_x_vertex_index_type;
		position_index_x_vertex_index_type position_index_x_vertex_index_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) {
			sources_list::process_child(child_name, child_id, child);

			if (child_name == "vertices") {
				vertices_.reset(construct<vertices>(child));
			}
			else if (child_name == "triangles") {
				triangles_.reset(construct<triangles>(child));
			}
		}

	public:
		void check() {
			if (!vertices_) throw std::runtime_error("no <vertices> found");
			if (!triangles_) throw std::runtime_error("no <triangles> found");
		}

		void build_mesh(memory_mesh *mesh_ptr) {
			input const *position_input = vertices_->get_input("POSITION");
			source const *position_source = get_source(position_input->get_source().substr(1));

			input const *normals_input = 0;
			try	{ normals_input = vertices_->get_input("NORMAL"); }
			catch(...) { normals_input = triangles_->get_input("NORMAL"); }

			source const *normals_source = get_source(normals_input->get_source().substr(1));

			input const *texcoords_input = triangles_->get_input("TEXCOORD");
			source const *texcoords_source = get_source(texcoords_input->get_source().substr(1));

			std::vector<size_t> const &indices = triangles_->get_indices();
			std::vector<size_t>::const_iterator indices_get = indices.begin();

			size_t stride = 1 + std::max(std::max(position_input->get_offset(), normals_input->get_offset()), texcoords_input->get_offset());

			for (size_t i = 0, n = triangles_->get_count() * 3; i < n; i++) {
				size_t position_index;
				memory_mesh::vertex v;

				for (size_t j = 0; j < stride; j++) {
					if (j == position_input->get_offset()) {
						position_index = *indices_get;
						position_source->get(*indices_get, v.position);
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

				if (vertex_index.second) {
					position_index_x_vertex_index_.insert(std::make_pair(position_index, vertex_index.first));
					mesh_ptr->vertices.push_back(v);
				}

				mesh_ptr->indices.push_back(vertex_index.first);
			}
		}

		void fix_position_indices(memory_mesh *mesh_ptr) {
			for (memory_mesh::influences_type::iterator mesh_it = mesh_ptr->influences.begin();
				mesh_it != mesh_ptr->influences.end(); mesh_it++)
			{
				std::vector<vertex_weight> weights;
				weights.swap(mesh_it->vertex_weights);

				for (std::vector<vertex_weight>::const_iterator it = weights.begin(); it != weights.end(); it++) {
					position_index_x_vertex_index_type::const_iterator jt = position_index_x_vertex_index_.lower_bound(std::make_pair(it->vertex_index, 0));
					while (jt != position_index_x_vertex_index_.end() && jt->first == it->vertex_index) {
						model::joint_influence::vertex_weight dummy = *it;
						dummy.vertex_index = jt->second;
						mesh_it->vertex_weights.push_back(dummy);
						jt++;
					}
				}
			}
		}
	};

	class geometry: public element {
	private:
		std::string id_, name_;
		boost::scoped_ptr<mesh> mesh_ptr_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) {
			if (child_name == "mesh") {
				mesh_ptr_.reset(construct<mesh>(child));
			}
		}

	public:
		std::string const &get_id() const { return id_; }
		std::string const &get_name() const { return name_; }

		void init(xercesc::DOMElement const *element_ptr) {
			id_ = xerces_get_attribute<std::string>(element_ptr, "id");
			name_ = xerces_get_attribute<std::string>(element_ptr, "name");
		}

		void check() {
			if (!mesh_ptr_) throw std::runtime_error("<mesh> not found in <geometry>");
		}

		void build_mesh(memory_mesh *mesh_ptr) {
			return mesh_ptr_->build_mesh(mesh_ptr);
		}

		void fix_position_indices(memory_mesh *mesh_ptr) {
			return mesh_ptr_->fix_position_indices(mesh_ptr);
		}
	};

	class library_geometries: public element {
	protected:
		boost::ptr_vector<geometry> geometries_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) {
			if (child_name == "geometry") {
				geometries_.push_back(construct<geometry>(child));
			}
		}

	public:
		void build_meshes(memory_model *model_ptr) {
			for (boost::ptr_vector<geometry>::iterator it = geometries_.begin(); it != geometries_.end(); it++) {
				model_ptr->meshes.push_back(new memory_mesh(model_ptr));
				memory_mesh *mesh_ptr = &model_ptr->meshes.back();
				mesh_ptr->id = it->get_id();
				mesh_ptr->name = it->get_name();
				it->build_mesh(mesh_ptr);
			}
		}

		void fix_position_indices(memory_model *model_ptr) {
			for (boost::ptr_vector<geometry>::iterator it = geometries_.begin(); it != geometries_.end(); it++) {
				it->fix_position_indices(model_ptr->get_mesh_by_id(it->get_id()));
			}
		}
	};

	class joints: public inputs_list {
	};

	class vertex_weights: public inputs_list {
	private:
		std::vector<size_t> vcount_;
		std::vector<size_t> v_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) {
			inputs_list::process_child(child_name, child_id, child);

			if (child_name == "vcount")
			{
				parse_list(child, vcount_);
			}
			else if (child_name == "v")
			{
				parse_list(child, v_);
			}
		}

	public:
		std::vector<size_t> const &get_vcount() const { return vcount_; }
		std::vector<size_t> const &get_v() const { return v_; }
	};

	class skin: public sources_list {
	private:
		math::matrix<4,4> bind_shape_matrix_;
		boost::scoped_ptr<joints> joints_;
		boost::scoped_ptr<vertex_weights> vertex_weights_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) {
			sources_list::process_child(child_name, child_id, child);

			if (child_name == "bind_shape_matrix") {
				parse_matrix(child, bind_shape_matrix_);
			}
			else if (child_name == "joints") {
				joints_.reset(construct<joints>(child));
			}
			else if (child_name == "vertex_weights") {
				vertex_weights_.reset(construct<vertex_weights>(child));
			}
		}

	public:
		void build_inverted_bind_matrices(memory_mesh *mesh_ptr)
		{
			mesh_ptr->bind_shape_matrix = bind_shape_matrix_;

			source const *joints_source = get_source(joints_->get_input("JOINT")->get_source().substr(1));
			source const *inverted_bind_matrices_source = get_source(joints_->get_input("INV_BIND_MATRIX")->get_source().substr(1));

			for (size_t i = 0, n = joints_source->get_size(); i < n; i++)
			{
				std::string joint_name;
				math::matrix<4,4> inv_bind_matrix;

				joints_source->get(i, joint_name);
				inverted_bind_matrices_source->get(i, inv_bind_matrix);

				mesh_ptr->get_joint_influence_by_name(joint_name, true)->inverted_bind_matrix = inv_bind_matrix;
			}
		}

		void build_joint_weights(memory_mesh *mesh_ptr)
		{
			input const *joints_input = vertex_weights_->get_input("JOINT");
			input const *weights_input = vertex_weights_->get_input("WEIGHT");

			source const *joints_source = get_source(joints_input->get_source().substr(1));
			source const *weights_source = get_source(weights_input->get_source().substr(1));

			size_t stride = 1 + std::max(joints_input->get_offset(), weights_input->get_offset());

			std::vector<size_t> const &vcount = vertex_weights_->get_vcount();
			std::vector<size_t> const &v = vertex_weights_->get_v();

			std::vector<size_t>::const_iterator v_get = v.begin();

			typedef std::map<size_t, std::vector<model::joint_influence::vertex_weight> > weights_type;
			weights_type weights;

			size_t position_index = 0;

			for (std::vector<size_t>::const_iterator it = vcount.begin(); it != vcount.end(); it++)
			{
				for (size_t i = 0; i < *it; i++)
				{
					size_t joint_index;
					float weight;

					for (size_t offset = 0; offset < stride; offset++)
					{
						if (offset == joints_input->get_offset())
						{
							joint_index = *v_get;
						}

						if (offset == weights_input->get_offset())
						{
							weights_source->get(*v_get, weight);
						}

						v_get++;
					}

					model::joint_influence::vertex_weight dummy;
					dummy.vertex_index = position_index;
					dummy.weight = weight;
					weights[joint_index].push_back(dummy);
				}

				position_index++;
			}

			for (weights_type::iterator it = weights.begin(); it != weights.end(); it++) {
				std::string joint_name;
				joints_source->get(it->first, joint_name);

				mesh_ptr->get_joint_influence_by_name(joint_name)->vertex_weights.swap(it->second);
			}
		}
	};

	class controller: public element {
	private:
		boost::ptr_map<std::string, skin *> skins_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) {
			skins_[xerces_get_attribute<std::string>(child, "source")] = construct<skin>(child);
		}

	public:
		std::string get_skin_source_id() const
		{
			if (skins_.size() != 1) throw std::logic_error("zero or more than one <skin> in <controller>");
			return skins_.begin()->first.substr(1);
		}

		void build_joint_influences(memory_model *model_ptr) {
			for (boost::ptr_map<std::string, skin *>::const_iterator it = skins_.begin(); it != skins_.end(); it++) {
				memory_mesh *mesh_ptr = model_ptr->get_mesh_by_id(it->first.substr(1));
				(*it->second)->build_inverted_bind_matrices(mesh_ptr);
				(*it->second)->build_joint_weights(mesh_ptr);
			}
		}
	};

	class library_controllers: public element {
	private:
		boost::ptr_map<std::string, controller *> controllers_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) {
			if (child_name == "controller") {
				controllers_[child_id] = construct<controller>(child);
			}
		}

	public:
		controller const *get_controller_by_id(std::string const &id) const
		{
			boost::ptr_map<std::string, controller *>::const_iterator it = controllers_.find(id);
			if (it != controllers_.end()) return *it->second;

			throw std::logic_error("controller with id = " + id + " not found");
		}

		void build_joint_influences(memory_model *model_ptr) {
			for (boost::ptr_map<std::string, controller *>::const_iterator it = controllers_.begin(); it != controllers_.end(); it++) {
				(*it->second)->build_joint_influences(model_ptr);
			}
		}
	};

	class sampler: public inputs_list {
	private:
		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) {
			inputs_list::process_child(child_name, child_id, child);
		}
	};

	class animation: public sources_list {
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
		boost::ptr_map<std::string, sampler *> samplers_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child)
		{
			sources_list::process_child(child_name, child_id, child);

			if (child_name == "sampler")
			{
				samplers_[child_id] = construct<sampler>(child);
			}
			else if (child_name == "channel")
			{
				channels_.push_back(channel(xerces_get_attribute<std::string>(child, "source"), xerces_get_attribute<std::string>(child, "target")));
			}
		}

	public:
		model::animation build_animation(memory_model *model_ptr)
		{
			model::animation result;

			for (std::vector<channel>::const_iterator it = channels_.begin(); it != channels_.end(); it++) {
				boost::ptr_map<std::string, sampler *>::iterator jt = samplers_.find(it->source_url.substr(1));
				if (jt == samplers_.end()) throw std::runtime_error("sampler for url " + it->source_url + " not found");

				sampler *sampler_ptr = *jt->second;

				source const *time_input = get_source(sampler_ptr->get_input("INPUT")->get_source().substr(1));
				source const *transform_output = get_source(sampler_ptr->get_input("OUTPUT")->get_source().substr(1));

				model::sampler dummy;
				transform const *transform_ptr = 0;

				// target is in form <node>/<transform>.<param>, <param> may be omitted
				std::vector<std::string> parsed_target_components;
				using namespace boost::lambda;
				boost::split(parsed_target_components, it->target_url, _1 == '/' || _1 == '.', boost::algorithm::token_compress_on);
				if (parsed_target_components.size() == 2) parsed_target_components.push_back(std::string());
				if (parsed_target_components.size() != 3) throw std::runtime_error("invalid url " + it->target_url);

				model::joint *joint_ptr = model_ptr->get_joint_by_id(parsed_target_components[0]);
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

	class library_animations: public element {
	private:
		boost::ptr_map<std::string, animation *> animations_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) {
			if (child_name == "animation") {
				animations_[child_id] = construct<animation>(child);
			}
		}

	public:
		void build_animations(memory_model *model_ptr) {
			for (boost::ptr_map<std::string, animation *>::iterator it = animations_.begin(); it != animations_.end(); it++) {
				model_ptr->animations.push_back(new model::animation((*it->second)->build_animation(model_ptr)));
				model_ptr->animations.back().name = it->first;
			}
		}
	};

	void node::build_hierarchy(memory_model *model_ptr, model::joint *parent_ptr)
	{
		model_ptr->joints.push_back(new model::joint());

		model::joint *joint_ptr = &model_ptr->joints.back();
		joint_ptr->id = id_;
		joint_ptr->name = sid_;
		joint_ptr->parent = parent_ptr;

		for (boost::ptr_vector<transform>::const_iterator it = transforms_.begin(); it != transforms_.end(); it++) {
			joint_ptr->transforms.push_back(model::joint::transform(it->get_sid(), it->get_baked_matrix()));
		}

		if (parent_ptr) parent_ptr->childs.push_back(joint_ptr);

		if (instance_geometry_.size())
		{
			joint_ptr->instance_ptr = model_ptr->get_mesh_by_id(instance_geometry_.substr(1));
		}
		else if (instance_controller_)
		{
			controller const *controller_ptr = get_root()->get_controllers()->get_controller_by_id(instance_controller_->get_url());
			joint_ptr->instance_ptr = model_ptr->get_mesh_by_id(controller_ptr->get_skin_source_id());
		}

		for (boost::ptr_vector<node>::iterator it = childs_.begin(); it != childs_.end(); it++) {
			it->build_hierarchy(model_ptr, joint_ptr);
		}
	}

	class collada: public element, public root_element {
	private:
		boost::scoped_ptr<library_visual_scenes> visual_scenes_;
		boost::scoped_ptr<library_geometries> geometries_;
		boost::scoped_ptr<library_controllers> controllers_;
		boost::scoped_ptr<library_animations> animations_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child)
		{
			if (child_name == "library_visual_scenes")
			{
				visual_scenes_.reset(construct<library_visual_scenes>(child));
			}
			else if (child_name == "library_geometries")
			{
				geometries_.reset(construct<library_geometries>(child));
			}
			else if (child_name == "library_controllers")
			{
				controllers_.reset(construct<library_controllers>(child));
			}
			else if (child_name == "library_animations")
			{
				animations_.reset(construct<library_animations>(child));
			}
		}

	public:
		collada()
		{
			set_root(this);
		}

		library_visual_scenes *get_visual_scenes()
		{
			return visual_scenes_.get();
		}

		library_controllers *get_controllers()
		{
			return controllers_.get();
		}

		void check()
		{
			if (!visual_scenes_) throw std::runtime_error("<library_visual_scenes> not found");
			if (!geometries_) throw std::runtime_error("<library_geometries> not found");
		}

		void build_model(memory_model *model_ptr)
		{
			geometries_->build_meshes(model_ptr);
			visual_scenes_->build_hierarchy(model_ptr);
			if (controllers_) controllers_->build_joint_influences(model_ptr);
			geometries_->fix_position_indices(model_ptr);
			if (animations_) animations_->build_animations(model_ptr);
		}
	};
}

namespace engine {
namespace graphic {

collada_loader::collada_loader(std::string const &filename):
filename_(filename)
{
}

collada_loader::~collada_loader() {
}

void collada_loader::load() {
	try {
		boost::scoped_ptr<xercesc::XercesDOMParser> parser(new xercesc::XercesDOMParser());
		parser->setValidationScheme(xercesc::XercesDOMParser::Val_Always);
		parser->setDoNamespaces(true);

		boost::scoped_ptr<xercesc::ErrorHandler> error_handler(new xercesc::HandlerBase());
		parser->setErrorHandler(error_handler.get());

		parser->parse(filename_.c_str());

		xercesc::DOMDocument *document = parser->getDocument();

		xercesc::DOMNodeList *node_list = document->getChildNodes();
		assert (node_list->getLength() == 1);

		xercesc::DOMNode *root_node = node_list->item(0);
		if (util::xerces_lcp_transcode(root_node->getLocalName()) != "COLLADA")
			throw load_error("root element is not <COLLADA />");

		collada_elements::collada dummy;
		dummy.go((xercesc::DOMElement *) root_node);
		dummy.build_model(&model_);
		model_.cleanup();
	}
	catch (std::exception const &error)
	{
		throw load_error("error loading collada " + filename_ + ": " + error.what());
	}
}

}
}
