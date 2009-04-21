
#include <map>
#include <string>
#include <sstream>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/scoped_ptr.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <math/matrix.h>
#include <engine/util/xerces_lcp_transcoder.h>
#include <engine/graphic/collada_importer.h>

namespace {
	using namespace engine::util;

	class element {
	public:
		element(xercesc::DOMElement const *element_ptr):
		element_ptr_(element_ptr)
		{
		}

		void go() {
			for (xercesc::DOMNode const *child_ptr = element_ptr_->getFirstChild();
				child_ptr != 0; child_ptr = child_ptr->getNextSibling())
			{
				if (child_ptr->getNodeType() == xercesc::DOMNode::ELEMENT_NODE) {
					process_child(xerces_lcp_transcode(child_ptr->getLocalName()),
						xerces_get_attribute<std::string>(child_ptr, "id"), (xercesc::DOMElement *) child_ptr);
				}
			}
		}

	protected:
		xercesc::DOMElement const *element_ptr_;

		virtual void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) = 0;
	};

	class geometry: public element {
	public:
		geometry(xercesc::DOMElement const *element_ptr):
		element(element_ptr)
		{
		}

	private:
		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) {

		}
	};

	class library_geometries: public element {
	public:
		library_geometries(xercesc::DOMElement const *element_ptr):
		element(element_ptr)
		{
		}

	private:
		boost::ptr_map<std::string, geometry *> geometries_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) {
			if (child_name == "geometry") {
				geometries_[child_id] = new geometry(child);
			}
		}
	};

	class collada: public element {
	public:
		collada(xercesc::DOMElement const *element_ptr):
		element(element_ptr)
		{
		}

	private:
		boost::scoped_ptr<library_geometries> geometries_;

		void process_child(std::string const &child_name, std::string const &child_id, xercesc::DOMElement const *child) {
			if (child_name == "library_geometries") {
				geometries_.reset(new library_geometries(child));
			}
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

	collada((xercesc::DOMElement *) root_node).go();
}

}
}
