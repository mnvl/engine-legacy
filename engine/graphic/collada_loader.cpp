
#include <map>
#include <string>
#include <sstream>
#include <memory>
#include <boost/ptr_container/ptr_map.hpp>
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
		std::string indices_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) {
			if (child_name == "p") {
				indices_ = xerces_get_text(child);
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
		enum array_type { FLOAT_ARRAY, NAME_ARRAY };
		array_type array_type_;
		std::string array_elements_;
		boost::scoped_ptr<technique_common> technique_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) {
			if (child_name == "float_array") {
				array_type_ = FLOAT_ARRAY;
				array_elements_ = xerces_get_text(child);
			}
			else if (child_name == "Name_array") {
				array_type_ = NAME_ARRAY;
				array_elements_ = xerces_get_text(child);
			}
			else if (child_name == "technique_common") {
				technique_.reset(construct<technique_common>(child));
			}
		}

	public:
		void check() {
			if (array_elements_.empty()) throw std::runtime_error("malformed or unknown <source>");
			if (!technique_) throw std::runtime_error("no <technique_common> in <source>");
		}
	};

	class mesh: public element {
	private:
		boost::ptr_map<std::string, source *> sources_;
		boost::scoped_ptr<vertices> vertices_;
		boost::scoped_ptr<triangles> triangles_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) {
			if (child_name == "source") {
				sources_[child_id] = construct<source>(child);
			}
			else if (child_name == "vertices") {
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

	class collada: public element {
	private:
		boost::scoped_ptr<library_geometries> geometries_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) {
			if (child_name == "library_geometries") {
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
