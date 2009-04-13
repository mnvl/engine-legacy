
#include <string>
#include <map>
#include <boost/scoped_ptr.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <engine/util/xerces_lcp_transcoder.h>
#include <engine/graphic/collada_importer.h>

namespace engine {
namespace graphic {

collada_importer::collada_importer(std::string const &filename):
filename_(filename)
{
}

void collada_importer::load() {
	boost::scoped_ptr<xercesc::XercesDOMParser> parser(new xercesc::XercesDOMParser());
    parser->setValidationScheme(xercesc::XercesDOMParser::Val_Always);
    parser->setDoNamespaces(true);

	boost::scoped_ptr<xercesc::ErrorHandler> error_handler(new xercesc::HandlerBase());
    parser->setErrorHandler(error_handler.get());

	parser->parse(filename_.c_str());

	xercesc::DOMDocument *document = parser->getDocument();

	xercesc::DOMNodeList *node_list = document->getChildNodes();
	assert(node_list->getLength() == 1);

	xercesc::DOMNode *node = node_list->item(0);
	assert (util::xerces_lcp_transcode(node_list->item(0)->getLocalName()) == "COLLADA");

	process_collada(node);
}

void collada_importer::process_collada(xercesc::DOMNode const *node) {
	for (xercesc::DOMNode const *child = node->getFirstChild(); child; child = child->getNextSibling()) {
		if (child->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;
		std::string const &node_name = util::xerces_lcp_transcode(child->getLocalName());

		if (node_name == "library_geometries")
		{
			process_library_geometries(child);
		}
		else
		{
		}
	}
}

void collada_importer::process_library_geometries(xercesc::DOMNode const *node) {
	for (xercesc::DOMNode const *child = node->getFirstChild(); child; child = child->getNextSibling()) {
		if (child->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;
		std::string const &node_name = util::xerces_lcp_transcode(child->getLocalName());

		if (node_name == "geometry")
		{
			process_geometry(child);
		}
		else
		{
		}
	}
}

void collada_importer::process_geometry(xercesc::DOMNode const *node) {
	for (xercesc::DOMNode const *child = node->getFirstChild(); child; child = child->getNextSibling()) {
		if (child->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;
		std::string const &node_name = util::xerces_lcp_transcode(child->getLocalName());

		if (node_name == "mesh")
		{
			process_mesh(child);
		}
		else
		{
		}
	}
}

void collada_importer::process_mesh(xercesc::DOMNode const *node) {
	std::map<std::string, xercesc::DOMNode const *> sources;

	for (xercesc::DOMNode const *child = node->getFirstChild(); child; child = child->getNextSibling()) {
		if (child->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;
		std::string const &node_name = util::xerces_lcp_transcode(child->getLocalName());

		if (node_name == "source")
		{
			sources.insert(std::make_pair(util::xerces_lcp_transcode(child->getAttributes()->
				getNamedItem(util::xerces_lcp_transcode("id").c_str())->getLocalName()), child));	
		}
		else if (node_name == "vertices")
		{
		}
	}
}

}
}