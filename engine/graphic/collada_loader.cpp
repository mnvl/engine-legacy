
#include <set>
#include <map>
#include <string>
#include <sstream>
#include <memory>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/scoped_ptr.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <math/matrix.h>
#include <engine/util/xerces_lcp_transcoder.h>
#include <engine/graphic/collada_loader.h>

#if defined(max)
#undef max
#endif

namespace collada_elements {
	using namespace engine::util;
	using namespace engine::graphic;

	class element {
	public:
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

		void init(xercesc::DOMElement const *element_ptr) {}
		virtual void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) {}
		void check() {}

		template <class T> static
		T *construct(xercesc::DOMElement const *element_ptr) {
			std::auto_ptr<T> holder(new T());
			holder->init(element_ptr);
			holder->go(element_ptr);
			holder->check();
			return holder.release();
		}

		template<class T> static void
		parse_list(xercesc::DOMElement const *element_ptr, std::vector<T> &list) {
			std::stringstream ss(xerces_get_text(element_ptr));
			T dummy;
			while (ss >> dummy) list.push_back(dummy);
			if (ss.bad()) throw std::runtime_error("can't parse list");
		}

		template<class T> static size_t
		parse_list(xercesc::DOMElement const *element_ptr, T *list, size_t size) {
			std::stringstream ss(xerces_get_text(element_ptr));
			size_t i = 0;
			while (i < size && ss >> *list++) i++;
			return i;
		}

		void parse_matrix(xercesc::DOMElement const *element_ptr, math::matrix<4, 4> &tf) {
			if (parse_list(element_ptr, &tf.ij[0][0], 16) != 16) throw std::runtime_error("invalid <matrix>");
			tf.transpose_inplace();
		}
	};

	class node: public element {
	protected:
		struct transform {
			std::string sid;
			math::matrix<4,4> matrix;
		};

		std::string id_, sid_, type_;
		node *parent_;
		boost::ptr_vector<node> childs_;
		std::vector<transform> transforms_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) {
			if (child_name == "node")
			{
				childs_.push_back(construct<node>(child));
				childs_.back().parent_ = this;
			}
			else if (child_name == "matrix")
			{
				transform tf;
				tf.sid = xerces_get_attribute<std::string>(child, "sid");
				parse_matrix(child, tf.matrix);
				transforms_.push_back(tf);
			}
		}

	public:
		void init(xercesc::DOMElement const *element_ptr) {
			id_ = xerces_get_attribute<std::string>(element_ptr, "id");
			sid_ = xerces_get_attribute<std::string>(element_ptr, "sid");
			type_ = xerces_get_attribute<std::string>(element_ptr, "type");
		}

		void check() {
			if (transforms_.empty()) throw std::runtime_error("no transforms defined for <node> with id = " + id_ + ", sid = " + sid_);
		}
	};

	class visual_scene: public node {
	public:
		void check() {
		}
	};

	class library_visual_scenes: public element {
	protected:
		boost::ptr_map<std::string, visual_scene *> visual_scenes_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) {
			if (child_name == "visual_scene") {
				visual_scenes_[child_id] = construct<visual_scene>(child);
			}
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
		boost::ptr_map<std::string, input *> inputs_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) {
			if (child_name == "input") {
				inputs_[xerces_get_attribute<std::string>(child, "semantic")] = construct<input>(child);
			}
		}

	public:
		input *get_input(std::string const &semantic) const {
			boost::ptr_map<std::string, input *>::const_iterator it = inputs_.find(semantic);
			if (it == inputs_.end()) throw std::runtime_error("input with semantic = " + semantic + " not found");
			return *it->second;
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
		}

		template <int N>
		void get(size_t index, math::vec<N> &v) {
			size_t array_index = index * technique_->get_stride();
			for (size_t i = 0; i < N; i++) {
				v.i[i] = float_array_.at(array_index++);
			}
		}
	};

	class sources_list: public element {
	protected:
		boost::ptr_map<std::string, source *> sources_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) {
			if (child_name == "source") {
				sources_[child_id] = construct<source>(child);
			}
		}

	public:
		source *get_source(std::string const &id) const {
			boost::ptr_map<std::string, source *>::const_iterator it = sources_.find(id);
			if (it == sources_.end()) throw std::runtime_error("source id = " + id + " not found");
			return *it->second;
		}
	};

	class vertex_indexer {
	private:
		struct vertex_less {
			bool operator ()(memory_mesh::vertex const &lhs, memory_mesh::vertex const &rhs) {
				if (lhs.position < rhs.position) return true;
				if (lhs.position > rhs.position) return false;

				if (lhs.normal < rhs.normal) return true;
				if (lhs.normal > rhs.normal) return false;

				return lhs.texcoords < rhs.texcoords;
			}
		};

		typedef std::map<memory_mesh::vertex, size_t, vertex_less> vertex_x_index_type;
		vertex_x_index_type vertex_x_index_;

	public:
		size_t choose_index(memory_mesh::vertex const &v) {
			vertex_x_index_type::const_iterator it = vertex_x_index_.find(v);
			if (it != vertex_x_index_.end()) return it->second;

			size_t index = vertex_x_index_.size();
			vertex_x_index_.insert(std::make_pair(v, index));
			return index;
		}
	};

	class mesh: public sources_list {
	private:
		boost::scoped_ptr<vertices> vertices_;
		boost::scoped_ptr<triangles> triangles_;

		vertex_indexer vertex_indexer_;
		std::set<std::pair<size_t, size_t> > position_index_x_vertex_index_;

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

		void build_mesh(memory_mesh &mesh) {
			input *position_input = vertices_->get_input("POSITION");
			source *position_source = get_source(position_input->get_source().substr(1));

			input *normals_input = 0;
			try	{ normals_input = vertices_->get_input("NORMAL"); }
			catch(...) { normals_input = triangles_->get_input("NORMAL"); }

			source *normals_source = get_source(normals_input->get_source().substr(1));

			input *texcoords_input = triangles_->get_input("TEXCOORD");
			source *texcoords_source = get_source(texcoords_input->get_source().substr(1));

			std::vector<size_t> const &indices = triangles_->get_indices();
			std::vector<size_t>::const_iterator indices_get = indices.begin();

			size_t stride = 1 + std::max(std::max(position_input->get_offset(), normals_input->get_offset()), texcoords_input->get_offset());

			for (size_t i = 0; i < triangles_->get_count(); i++) {
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
				}

				size_t vertex_index = vertex_indexer_.choose_index(v);
				position_index_x_vertex_index_.insert(std::make_pair(position_index, vertex_index));

				mesh.vertices.push_back(v);
				mesh.indices.push_back(vertex_index);
			}
		}
	};

	class geometry: public element {
	private:
		boost::scoped_ptr<mesh> mesh_ptr_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) {
			if (child_name == "mesh") {
				mesh_ptr_.reset(construct<mesh>(child));
			}
		}

	public:
		void check() {
			if (!mesh_ptr_) throw std::runtime_error("<mesh> not found in <geometry>");
		}

		void build_mesh(memory_mesh &mesh) {
			return mesh_ptr_->build_mesh(mesh);
		}
	};

	class library_geometries: public element {
	protected:
		boost::ptr_map<std::string, geometry *> geometries_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) {
			if (child_name == "geometry") {
				geometries_[child_id] = construct<geometry>(child);
			}
		}

	public:
		void build_meshes(memory_model *model_ptr) {
			for (boost::ptr_map<std::string, geometry *>::const_iterator it = geometries_.begin(); it != geometries_.end(); it++) {
				memory_mesh mesh;
				mesh.name = it->first;
				(*it->second)->build_mesh(mesh);
				model_ptr->meshes.push_back(mesh);
			}
		}
	};

	class joints: public inputs_list {
	};

	class vertex_weights: public inputs_list {
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
	};

	class controller: public element {
	private:
		boost::ptr_map<std::string, skin *> skins_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) {
			skins_[xerces_get_attribute<std::string>(child, "source")] = construct<skin>(child);
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
	};

	class sampler: public inputs_list {
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

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) {
			inputs_list::process_child(child_name, child_id, child);

			if (child_name == "channel") {
				channels_.push_back(channel(xerces_get_attribute<std::string>(child, "source"), xerces_get_attribute<std::string>(child, "target")));
			}
		}
	};

	class animation: public sources_list {
	private:
		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) {
			sources_list::process_child(child_name, child_id, child);
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
	};

	class collada: public element {
	private:
		boost::scoped_ptr<library_visual_scenes> visual_scenes_;
		boost::scoped_ptr<library_geometries> geometries_;
		boost::scoped_ptr<library_controllers> controllers_;
		boost::scoped_ptr<library_animations> animations_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) {
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
		void check() {
			if (!geometries_) throw std::runtime_error("<library_geometries> not found");
		}

		void build_model(memory_model *model_ptr) {
			geometries_->build_meshes(model_ptr);
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
	}
	catch (std::exception const &error)
	{
		throw load_error("error loading collada " + filename_ + ": " + error.what());
	}
}

}
}
