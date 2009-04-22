
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

namespace collada_elements {
	using namespace engine::util;

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
			while (i < size) ss >> *list++;

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
			if (child_name == "node") {
				childs_.push_back(construct<node>(child));
				childs_.back().parent_ = this;
			}
			else if (child_name == "matrix") {
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
			if (transforms_.empty()) throw std::runtime_error("no transforms defined for <node>");
		}
	};

	class visual_scene: public node {
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

	class inputs_list: public element {
	protected:
		std::map<std::string, std::string> inputs_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) {
			if (child_name == "input") {
				inputs_[xerces_get_attribute<std::string>(child, "semantic")] = xerces_get_attribute<std::string>(child, "source");
			}
		}
	};

	class vertices: public inputs_list {
	};

	class triangles: public inputs_list {
	private:
		std::vector<size_t> indices_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) {
			if (child_name == "p") {
				parse_list(child, indices_);
			}

			inputs_list::process_child(child_name, child_id, child);
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

		size_t count_, stide_;
		std::vector<param> params_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) {
			if (child_name == "acessor") {
				count_ = xerces_get_attribute<size_t>(child, "count");
				count_ = xerces_get_attribute<size_t>(child, "stride");

				xercesc::DOMNodeList const *params = child->getElementsByTagName(xerces_lcp_transcode("param").c_str());
				for (size_t i = 0, n = params->getLength(); i < n; i++) {
					xercesc::DOMNode const *node = params->item(i);
					params_.push_back(param(xerces_get_attribute<std::string>(node, "name"), xerces_get_attribute<std::string>(node, "type")));
				}
			}
		}
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
		void check() {
			if (float_array_.empty() && name_array_.empty()) throw std::runtime_error("malformed or unknown <source>");
			if (!technique_) throw std::runtime_error("no <technique_common> in <source>");
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
	};

	class mesh: public sources_list {
	private:
		boost::scoped_ptr<vertices> vertices_;
		boost::scoped_ptr<triangles> triangles_;

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
	};

	class library_geometries: public element {
	protected:
		boost::ptr_map<std::string, geometry *> geometries_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) {
			if (child_name == "geometry") {
				geometries_[child_id] = construct<geometry>(child);
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

	class collada: public element {
	private:
		boost::scoped_ptr<library_visual_scenes> visual_scenes_;
		boost::scoped_ptr<library_geometries> geometries_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) {
			if (child_name == "library_visual_scenes") {
				visual_scenes_.reset(construct<library_visual_scenes>(child));
			} else if (child_name == "library_geometries") {
				geometries_.reset(construct<library_geometries>(child));
			}
		}

	public:
		void check() {
			if (!geometries_) throw std::runtime_error("<library_geometries> not found");
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

		collada_elements::collada().go((xercesc::DOMElement *) root_node);
	}
	catch (std::exception const &error)
	{
		throw load_error("error loading collada " + filename_ + ": " + error.what());
	}
}

}
}
