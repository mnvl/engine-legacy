
#include <boost/scoped_ptr.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <engine/util/xerces_lcp_transcoder.h>
#include <engine/graphic/collada_importer.h>

namespace engine {
namespace graphic {

void collada_importer::load(char const *filename) {
	boost::scoped_ptr<xercesc::XercesDOMParser> parser(new xercesc::XercesDOMParser());
    parser->setValidationScheme(xercesc::XercesDOMParser::Val_Always);
    parser->setDoNamespaces(true);

	boost::scoped_ptr<xercesc::ErrorHandler> error_handler(new xercesc::HandlerBase());
    parser->setErrorHandler(error_handler.get());

	parser->parse(filename);

	xercesc::DOMDocument *document = parser->getDocument();

	xercesc::DOMNodeList *node_list = document->getChildNodes();
	assert(node_list->getLength() == 1);

	xercesc::DOMNode *node = node_list->item(0);
	assert (util::xerces_lcp_transcode(node_list->item(0)->getLocalName()) == "COLLADA");
}

void collada_importer::process_collada(xercesc::DOMNode const *node) {
	for(xercesc::DOMNode const *child = node->getFirstChild(); child; child = child->getNextSibling()) {
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
}

}
}