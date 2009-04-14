
#include <string>
#include <map>
#include <sstream>
#include <boost/scoped_ptr.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <engine/util/xerces_lcp_transcoder.h>
#include <engine/graphic/collada_importer.h>

namespace engine {
namespace graphic {

collada_importer::collada_importer(std::string const &filename, mesh *mesh_ptr):
filename_(filename),
mesh_ptr_(mesh_ptr)
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
		std::string const &child_name = util::xerces_lcp_transcode(child->getLocalName());

		if (child_name == "library_geometries")
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
		std::string const &child_name = util::xerces_lcp_transcode(child->getLocalName());

		if (child_name == "geometry")
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
		std::string const &child_name = util::xerces_lcp_transcode(child->getLocalName());

		if (child_name == "mesh")
		{
			process_mesh(child);
		}
		else
		{
		}
	}
}

void collada_importer::process_mesh(xercesc::DOMNode const *node) {
	for (xercesc::DOMNode const *child = node->getFirstChild(); child; child = child->getNextSibling()) {
		if (child->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;
		std::string const &child_name = util::xerces_lcp_transcode(child->getLocalName());

		if (child_name == "source")
		{
			process_source(child, &sources_[util::xerces_get_attribute<std::string>(child, "id")]);	
		}
		else if (child_name == "vertices")
		{
			process_vertices(child);
		}
		else if (child_name == "triangles")
		{
			process_triangles(child);
		}
		else
		{
		}
	}
}

void collada_importer::process_vertices(xercesc::DOMNode const *node) {
	for (xercesc::DOMNode const *child = node->getFirstChild(); child; child = child->getNextSibling()) {
		if (child->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;
		std::string const &child_name = util::xerces_lcp_transcode(child->getLocalName());

		if (child_name == "input")
		{
			input_info dummy;
			dummy.source_name = util::xerces_get_attribute<std::string>(child, "source");

			inputs_[util::xerces_get_attribute<std::string>(child, "semantic")] = dummy;
		}
		else
		{
		}
	}
}

void collada_importer::process_source(xercesc::DOMNode const *node, source_info *si)
{
	for (xercesc::DOMNode const *child = node->getFirstChild(); child; child = child->getNextSibling()) {
		if (child->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;
		std::string const &child_name = util::xerces_lcp_transcode(child->getLocalName());

		if (child_name == "float_array")
		{
			std::stringstream ss(util::xerces_get_text(child));

			math::scalar val;
			while (ss >> val) { si->data.push_back(val); }
		}
		else if (child_name == "technique_common")
		{
			process_technique(child, &si->technique);
		}
		else
		{
		}
	}

	if (si->data.size() != si->technique.count * si->technique.stride)
		throw std::runtime_error("malformed float_array in " + filename_);
}

void collada_importer::process_technique(xercesc::DOMNode const *node, technique_info *ti)
{
	for (xercesc::DOMNode const *child = node->getFirstChild(); child; child = child->getNextSibling()) {
		if (child->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;
		std::string const &child_name = util::xerces_lcp_transcode(child->getLocalName());

		if (child_name == "accessor")
		{
			ti->count = util::xerces_get_attribute<size_t>(child, "count");
			ti->stride = util::xerces_get_attribute<size_t>(child, "stride");
		}
		else
		{
		}
	}
}

void collada_importer::process_triangles(xercesc::DOMNode const *node)
{
	size_t count = util::xerces_get_attribute<size_t>(node, "count");

	for (xercesc::DOMNode const *child = node->getFirstChild(); child; child = child->getNextSibling()) {
		if (child->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;
		std::string const &child_name = util::xerces_lcp_transcode(child->getLocalName());

		if (child_name == "input")
		{
			input_info dummy;
			dummy.source_name = util::xerces_get_attribute<std::string>(child, "source");
			dummy.offset = util::xerces_get_attribute<size_t>(child, "offset");

			inputs_[util::xerces_get_attribute<std::string>(child, "semantic")] = dummy;
		}
		else
		{
		}
	}
}

}
}