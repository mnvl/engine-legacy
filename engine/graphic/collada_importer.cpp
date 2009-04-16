
#include <string>
#include <map>
#include <sstream>
#include <boost/scoped_ptr.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <math/matrix.h>
#include <engine/util/xerces_lcp_transcoder.h>
#include <engine/graphic/collada_importer.h>

namespace engine {
namespace graphic {

collada_importer::collada_importer(std::string const &filename):
filename_(filename),
mesh_ptr_(0)
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

void collada_importer::clear() {
	sources_.clear();
	inputs_.clear();
}

collada_importer::source_info const &collada_importer::indirect_source(xercesc::DOMNode const *input_node) {
	std::string const &source = util::xerces_get_attribute<std::string>(input_node, "source").substr(1);
	std::map<std::string, source_info>::const_iterator it = sources_.find(source);
	if (it == sources_.end()) throw std::runtime_error("source " + source + " not found in " + filename_);
	return it->second;
}

void collada_importer::process_collada(xercesc::DOMNode const *node) {
	for (xercesc::DOMNode const *child = node->getFirstChild(); child; child = child->getNextSibling()) {
		if (child->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;
		std::string const &child_name = util::xerces_lcp_transcode(child->getLocalName());

		if (child_name == "library_geometries")
		{
			process_library_geometries(child);
		}
		if (child_name == "library_controllers")
		{
			process_library_controllers(child);
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
	clear();

	std::string const &mesh_id = util::xerces_get_attribute<std::string>(node, "id");
	mesh_ptr_ = &builded_meshes_.insert(std::make_pair(mesh_id, memory_mesh())).first->second;

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

	clear();
}

void collada_importer::process_mesh(xercesc::DOMNode const *node) {
	for (xercesc::DOMNode const *child = node->getFirstChild(); child; child = child->getNextSibling()) {
		if (child->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;
		std::string const &child_name = util::xerces_lcp_transcode(child->getLocalName());

		if (child_name == "source")
		{
			process_source(child);	
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

void collada_importer::process_vertices(xercesc::DOMNode const *node)
{
	process_inputs(node);
}

void collada_importer::process_inputs(xercesc::DOMNode const *node)
{
	for (xercesc::DOMNode const *child = node->getFirstChild(); child; child = child->getNextSibling()) {
		if (child->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;
		std::string const &child_name = util::xerces_lcp_transcode(child->getLocalName());

		if (child_name == "input")
		{
			input_info dummy;
			dummy.source_name = util::xerces_get_attribute<std::string>(child, "source");
			dummy.offset = 0;

			inputs_[util::xerces_get_attribute<std::string>(child, "semantic")] = dummy;
		}
		else
		{
		}
	}
}

void collada_importer::process_source(xercesc::DOMNode const *node)
{
	source_info *si = &sources_[util::xerces_get_attribute<std::string>(node, "id")];
	bool nocheck = false;

	for (xercesc::DOMNode const *child = node->getFirstChild(); child; child = child->getNextSibling()) {
		if (child->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;
		std::string const &child_name = util::xerces_lcp_transcode(child->getLocalName());

		if (child_name == "float_array")
		{
			std::stringstream ss(util::xerces_get_text(child));

			math::scalar val;
			while (ss >> val) { si->data.push_back(val); }
		}
		else if (child_name == "Name_array")
		{
			nocheck = true;
		}
		else if (child_name == "technique_common")
		{
			process_technique(child, &si->technique);
		}
		else
		{
		}
	}

	if (nocheck) return;

	if (si->data.size() != si->technique.count * si->technique.stride) {
		throw std::runtime_error("malformed float_array in source "
			+ util::xerces_get_attribute<std::string>(node, "id") 
			+ " of file " + filename_);
	}
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
		else if (child_name == "p")
		{
			std::stringstream ss(util::xerces_get_text(child));
			std::vector<unsigned> indices;
			
			unsigned index;
			while (ss >> index) indices.push_back(index);

			build_mesh(indices);
		}
		else
		{
		}
	}
}

void collada_importer::build_mesh(std::vector<unsigned> const &indices)
{
	input_info const &positions = inputs_["POSITION"];
	input_info const &normals = inputs_["NORMAL"];
	input_info const &texcoords = inputs_["TEXCOORD"];

	source_info const &positions_source = sources_[positions.source_name.substr(1)];
	source_info const &normals_source = sources_[normals.source_name.substr(1)];
	source_info const &texcoords_source = sources_[texcoords.source_name.substr(1)];

	size_t stride = positions.offset;
	if (normals.offset > stride) stride = normals.offset;
	if (texcoords.offset > stride) stride = texcoords.offset;
	stride++;

	std::vector<mesh::vertex> face_vertices;

	for (size_t index = 0; index < indices.size(); index += stride)
	{
		unsigned data_filled = 0;
		mesh::vertex v;

		for (size_t offset = 0; offset < stride; offset++)
		{
			if (offset == positions.offset)
			{
				positions_source.fill_vector(v.position, indices[index + offset]);
				data_filled |= 1;
			}

			if (offset == normals.offset)
			{
				normals_source.fill_vector(v.normal, indices[index + offset]);
				data_filled |= 2;
			}

			if (offset == texcoords.offset)
			{
				texcoords_source.fill_vector(v.texcoords, indices[index + offset]);
				data_filled |= 4;
			}
		}

		if (data_filled != 7)
			throw std::runtime_error("error importing " + filename_ + ": vertex is not filled");

		face_vertices.push_back(v);

		if (face_vertices.size() == 3) {
			for (size_t i = 0; i < 3; i++) {
				mesh_ptr_->add_index(mesh_ptr_->add_vertex(face_vertices[i]));
			}

			face_vertices.clear();
		}
	}
}

void collada_importer::process_library_controllers(xercesc::DOMNode const *node)
{
	for (xercesc::DOMNode const *child = node->getFirstChild(); child; child = child->getNextSibling())
	{
		if (child->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;
		std::string const &child_name = util::xerces_lcp_transcode(child->getLocalName());

		if (child_name == "controller")
		{
			process_controller(child);
		}
		else
		{
		}
	}
}

void collada_importer::process_controller(xercesc::DOMNode const *node)
{
	clear();

	for (xercesc::DOMNode const *child = node->getFirstChild(); child; child = child->getNextSibling())
	{
		if (child->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;
		std::string const &child_name = util::xerces_lcp_transcode(child->getLocalName());

		if (child_name == "skin")
		{
			process_skin(child);
		}
		else
		{

		}
	}
	
	clear();
}

void collada_importer::process_skin(xercesc::DOMNode const *node)
{
	std::string const &source = util::xerces_get_attribute<std::string>(node, "source");
	mesh_ptr_ = &builded_meshes_.find(source.substr(1))->second;

	math::matrix<4, 4> bind_shape_matrix;

	for (xercesc::DOMNode const *child = node->getFirstChild(); child; child = child->getNextSibling())
	{
		if (child->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;
		std::string const &child_name = util::xerces_lcp_transcode(child->getLocalName());

		if (child_name == "bind_shape_matrix")
		{
			std::stringstream ss(util::xerces_get_text(node));

			for (size_t i = 0; i < 4; i++)
			{
				for (size_t j = 0; j < 4; j++)
				{
					ss >> bind_shape_matrix.ij[i][j];
				}
			}

			if (ss.bad()) throw std::runtime_error("error parsing bind shape matrix for "
				+ source + " in file " + filename_);
		}
		else if (child_name == "source")
		{
			process_source(child);
		}
		else if (child_name == "joints")
		{
			process_joints(child);
		}
		else if (child_name == "vertex_weights")
		{
			process_vertex_weights(child);
		}
		else
		{
		}
	}
}

void collada_importer::process_joints(xercesc::DOMNode const *node)
{
	for (xercesc::DOMNode const *child = node->getFirstChild(); child; child = child->getNextSibling())
	{
		if (child->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;
		std::string const &child_name = util::xerces_lcp_transcode(child->getLocalName());

		if (child_name == "input")
		{
			std::string const &semantic = util::xerces_get_attribute<std::string>(child, "semantic");

			if (semantic == "JOINT")
			{
				source_info const &source = indirect_source(child);
				mesh_ptr_->resize_joints_buffer(source.technique.count);
			}
			else if (semantic == "INV_BIND_MATRIX")
			{
				source_info const &source = indirect_source(child);

				mesh::joint *joints_buffer = mesh_ptr_->lock_joints_buffer();
				std::vector<float>::const_iterator get = source.data.begin();

				for (size_t i = 0; i <mesh_ptr_->get_joints_count(); i++)
				{
					mesh::joint *joint_ptr = joints_buffer + i;

					for (size_t i = 0; i < 4; i++) {
						for (size_t j = 0; j < 4; j++) {
							joint_ptr->inverted_bind_matrix.ij[j][i] = *get++;
						}
					}
				}

				mesh_ptr_->unlock_joints_buffer(joints_buffer);
			}
		}
		else
		{
		}
	}
}

void collada_importer::process_vertex_weights(xercesc::DOMNode const *node)
{

}

}
}
