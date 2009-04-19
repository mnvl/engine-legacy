
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

void collada_importer::clear() {
	sources_.clear();
	inputs_.clear();
	sampler_by_name_.clear();
}

collada_importer::source_info const &collada_importer::indirect_source(xercesc::DOMNode const *input_node) {
	std::string const &source = util::xerces_get_attribute<std::string>(input_node, "source").substr(1);
	std::map<std::string, source_info>::const_iterator it = sources_.find(source);
	if (it == sources_.end()) throw import_error("source " + source + " not found in " + filename_);
	return it->second;
}

void collada_importer::process_collada(xercesc::DOMNode const *node) {
	for (int pass = 0; pass < 4; pass++) {
		for (xercesc::DOMNode const *child = node->getFirstChild(); child; child = child->getNextSibling()) {
			if (child->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;
			std::string const &child_name = util::xerces_lcp_transcode(child->getLocalName());

			if (pass == 1 && child_name == "library_visual_scenes")
			{
				process_library_visual_scenes(child);
			}

			if (pass == 0 && child_name == "library_geometries")
			{
				process_library_geometries(child);
			}

			if (pass == 2 && child_name == "library_controllers")
			{
				process_library_controllers(child);
			}

			if (pass == 3 && child_name == "library_animations")
			{
				process_library_animations(child);
			}
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
	}
}

void collada_importer::process_geometry(xercesc::DOMNode const *node) {
	clear();

	std::string const &mesh_id = util::xerces_get_attribute<std::string>(node, "id");

	{
		model_.meshes.push_back(memory_mesh());
		model_.meshes.back().name = mesh_id;

		mesh_in_progress_ = &meshes_in_progress_.insert(std::make_pair(mesh_id, mesh_in_progress(&model_.meshes.back()))).first->second;
	}

	for (xercesc::DOMNode const *child = node->getFirstChild(); child; child = child->getNextSibling()) {
		if (child->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;
		std::string const &child_name = util::xerces_lcp_transcode(child->getLocalName());

		if (child_name == "mesh")
		{
			process_mesh(child);
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
	}
}

void collada_importer::process_source(xercesc::DOMNode const *node)
{
	source_info *si = &sources_[util::xerces_get_attribute<std::string>(node, "id")];

	for (xercesc::DOMNode const *child = node->getFirstChild(); child; child = child->getNextSibling()) {
		if (child->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;
		std::string const &child_name = util::xerces_lcp_transcode(child->getLocalName());

		if (child_name == "float_array")
		{
			std::stringstream ss(util::xerces_get_text(child));

			math::scalar val;
			while (ss >> val) { si->float_array.push_back(val); }
		}
		else if (child_name == "Name_array")
		{
			std::stringstream ss(util::xerces_get_text(child));

			std::string val;
			while (ss >> val) { si->name_array.push_back(val); }
		}
		else if (child_name == "technique_common")
		{
			process_technique(child, &si->technique);
		}
	}

	if (si->float_array.size() && si->float_array.size() != si->technique.count * si->technique.stride) {
		throw import_error("malformed float_array in source "
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

			process_accessor(child, ti);
		}
	}
}

void collada_importer::process_accessor(xercesc::DOMNode const *node, technique_info *ti)
{
	for (xercesc::DOMNode const *child = node->getFirstChild(); child; child = child->getNextSibling()) {
		if (child->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;
		std::string const &child_name = util::xerces_lcp_transcode(child->getLocalName());

		if (child_name == "param")
		{
			ti->params.insert(std::make_pair(util::xerces_get_attribute<std::string>(child, "name"),
				util::xerces_get_attribute<std::string>(child, "type")));
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
	}
}

void collada_importer::process_controller(xercesc::DOMNode const *node)
{
	clear();

	std::string const &id = util::xerces_get_attribute<std::string>(node, "id");

	for (xercesc::DOMNode const *child = node->getFirstChild(); child; child = child->getNextSibling())
	{
		if (child->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;
		std::string const &child_name = util::xerces_lcp_transcode(child->getLocalName());

		if (child_name == "skin")
		{
			mesh_by_instance_controller_[id] = util::xerces_get_attribute<std::string>(child, "source").substr(1);
			process_skin(child);
		}
	}
	
	clear();
}

void collada_importer::process_skin(xercesc::DOMNode const *node)
{
	std::string const &source = util::xerces_get_attribute<std::string>(node, "source");
	mesh_in_progress_ = &meshes_in_progress_.find(source.substr(1))->second;
	mesh_in_progress_->mesh_ptr = model_.get_mesh_by_name(source.substr(1));

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

			if (ss.bad()) throw import_error("error parsing bind shape matrix for "
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
				mesh_in_progress_->mesh_ptr->influences.resize(source.name_array.size());

				joint_influence *influences_ptr = &mesh_in_progress_->mesh_ptr->influences[0];

				for (size_t i = 0, n = source.name_array.size(); i < n; i++) {
					influences_ptr->joint_ptr = model_.get_joint_by_name(source.name_array[i]);
					if (influences_ptr->joint_ptr == 0) throw import_error("joint " + source.name_array[i] + " not found");
				}
			}
			else if (semantic == "INV_BIND_MATRIX")
			{
				source_info const &source = indirect_source(child);

				std::vector<float>::const_iterator get = source.float_array.begin();

				for (size_t i = 0; i <mesh_in_progress_->mesh_ptr->influences.size(); i++)
				{
					joint_influence *joint_influence_ptr = &mesh_in_progress_->mesh_ptr->influences[i];

					for (size_t i = 0; i < 4; i++) {
						for (size_t j = 0; j < 4; j++) {
							joint_influence_ptr->inverted_bind_matrix.ij[j][i] = *get++;
						}
					}
				}
			}
		}
	}
}

void collada_importer::process_vertex_weights(xercesc::DOMNode const *node)
{
	source_info const *joint_source = 0, *weight_source = 0;
	size_t joint_offset, weight_offset;
	std::vector<size_t> vcount, v;

	for (xercesc::DOMNode const *child = node->getFirstChild(); child; child = child->getNextSibling())
	{
		if (child->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;
		std::string const &child_name = util::xerces_lcp_transcode(child->getLocalName());

		if (child_name == "input")
		{
			std::string const &semantic = util::xerces_get_attribute<std::string>(child, "semantic");

			if (semantic == "JOINT")
			{
				joint_source = &indirect_source(child);
				joint_offset = util::xerces_get_attribute<size_t>(child, "offset");
			}
			else if (semantic == "WEIGHT")
			{
				weight_source = &indirect_source(child);
				weight_offset = util::xerces_get_attribute<size_t>(child, "offset");
			}
		}
		else if (child_name == "vcount")
		{
			std::stringstream ss(util::xerces_get_text(child));
			size_t count;
			while (ss >> count) vcount.push_back(count);
		}
		else if (child_name == "v")
		{
			std::stringstream ss(util::xerces_get_text(child));
			size_t item;
			while (ss >> item) v.push_back(item);
		}
	}

	size_t stride = max(joint_offset, weight_offset) + 1;
	std::vector<size_t>::const_iterator get = v.begin();

	for (size_t i = 0; i < vcount.size(); i++)
	{
		for (size_t j = 0, n = vcount[i]; j < n; j++)
		{
			size_t joint_index;
			float weight;

			for (size_t k = 0; k < stride; k++)
			{
				if (k == joint_offset)
				{
					joint_index = *get;
				}

				if (k == weight_offset)
				{
					weight = weight_source->float_array[*get];
				}

				get++;
			}

			joint_influence::vertex_weight dummy;
			dummy.weight = weight;

			for (mesh_in_progress::position_index_transform_type::const_iterator it = mesh_in_progress_->position_index_transform.find(i);
				it != mesh_in_progress_->position_index_transform.end() && it->first == i; it++)
			{
				dummy.vertex_index = it->second;
				mesh_in_progress_->mesh_ptr->influences[joint_index].vertex_weights.push_back(dummy);
			}
		}
	}
}

void collada_importer::process_library_visual_scenes(xercesc::DOMNode const *node)
{
	for (xercesc::DOMNode const *child = node->getFirstChild(); child; child = child->getNextSibling())
	{
		if (child->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;
		std::string const &child_name = util::xerces_lcp_transcode(child->getLocalName());

		if (child_name == "visual_scene")
		{
			process_visual_scene(child);
		}
	}
}

void collada_importer::process_visual_scene(xercesc::DOMNode const *node)
{
	for (xercesc::DOMNode const *child = node->getFirstChild(); child; child = child->getNextSibling())
	{
		if (child->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;
		std::string const &child_name = util::xerces_lcp_transcode(child->getLocalName());

		if (child_name == "node")
		{
			process_node(node);
		}
	}
}

void collada_importer::process_node(xercesc::DOMNode const *node, joint *parent)
{
	std::string const &node_type = util::xerces_get_attribute<std::string>(node, "type");
	memory_mesh *mesh_ptr = 0;

	if (node_type == "JOINT")
	{
		joint dummy;
		dummy.name = util::xerces_get_attribute<std::string>(node, "id");
		dummy.parent = parent;
		model_.joints.push_back(dummy);
	}

	math::matrix<4, 4> transform;
	transform.identity();

	for (xercesc::DOMNode const *child = node->getFirstChild(); child; child = child->getNextSibling())
	{
		if (child->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;
		std::string const &child_name = util::xerces_lcp_transcode(child->getLocalName());

		if (child_name == "node")
		{
			process_node(child);
		}
		else if (child_name == "instance_controller")
		{
			std::string const &url = util::xerces_get_attribute<std::string>(child, "url");
			mesh_ptr = model_.get_mesh_by_name(mesh_by_instance_controller_.find(url.substr(1))->second);
		}
		else if (child_name == "translate")
		{
			std::stringstream ss(util::xerces_get_text(child));
			math::vec<3> v;
			ss >> v;
			transform.translate(v);
		}
		else if (child_name == "matrix")
		{
			std::stringstream ss(util::xerces_get_text(child));
			math::matrix<4,4> m;
			ss >> m;
			m.transpose_inplace();

			transform *= m;
		}
	}

	if (mesh_ptr) {
		mesh_ptr->transform = transform;

		for (size_t i = 0; i < mesh_ptr->vertices.size(); i++) {
			mesh_ptr->vertices[i].position = mesh_ptr->vertices[i].position * transform;
		}
	}
}

void collada_importer::process_library_animations(xercesc::DOMNode const *node)
{
	for (xercesc::DOMNode const *child = node->getFirstChild(); child; child = child->getNextSibling())
	{
		if (child->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;
		std::string const &child_name = util::xerces_lcp_transcode(child->getLocalName());

		if (child_name == "animation")
		{
			process_animation(child);
		}
	}
}

void collada_importer::process_animation(xercesc::DOMNode const *node)
{
	clear();

	model_.animations.push_back(animation());

	for (xercesc::DOMNode const *child = node->getFirstChild(); child; child = child->getNextSibling())
	{
		if (child->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;
		std::string const &child_name = util::xerces_lcp_transcode(child->getLocalName());

		if (child_name == "source")
		{
			process_source(child);	
		}
		else if (child_name == "sampler")
		{
			process_sampler(child);
		}
		else if (child_name == "channel")
		{
			sampler *sampler_ptr = &sampler_by_name_.find(util::xerces_get_attribute<std::string>(child, "source").substr(1))->second;
			std::string const &target = util::xerces_get_attribute<std::string>(child, "target");

			sampler_ptr->joint_ptr = model_.get_joint_by_name(target.substr(0, target.find('/')));
			//if (!sampler_ptr->joint_ptr) throw import_error("target " + target + " not found");

			model_.animations.back().samplers.push_back(*sampler_ptr);
		}
	}

	clear();
}

void collada_importer::process_sampler(xercesc::DOMNode const *node)
{
	source_info const *input = 0;
	source_info const *output = 0;
	source_info const *interpolation = 0;

	std::string const &name = util::xerces_get_attribute<std::string>(node, "id");
	sampler *sampler_ptr = &sampler_by_name_.insert(std::make_pair(name, sampler())).first->second;

	for (xercesc::DOMNode const *child = node->getFirstChild(); child; child = child->getNextSibling())
	{
		if (child->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;
		std::string const &child_name = util::xerces_lcp_transcode(child->getLocalName());

		if (child_name == "input")
		{
			std::string const &semantic = util::xerces_get_attribute<std::string>(child, "semantic");
			std::string const &source_name = util::xerces_get_attribute<std::string>(child, "source");
			source_info const *source_ptr = &indirect_source(child);

			if (semantic == "INPUT") input = source_ptr;
			else if (semantic == "OUTPUT") output = source_ptr;
			else if (semantic == "INTERPOLATION") interpolation = source_ptr;
		}
	}

	if (!input || !output || !interpolation) {
		throw import_error("one of sources of sampler " + name + " not found");
	}

	bool output_is_matrix = false;

	if (output->technique.params.find("TRANSFORM") != output->technique.params.end())
	{
		output_is_matrix = true;
	}
	else
	{
		throw import_error("invalid output param of sampler " + name);
	}

	std::vector<float>::const_iterator output_get = output->float_array.begin();

	for (size_t sample_index = 0; sample_index < input->technique.count; sample_index++) {
		sampler::sample s;

		s.time = input->float_array[sample_index];

		if (output_is_matrix) {
			for (size_t i = 0; i < 4; i++) {
				for (size_t j = 0; j < 4; j++) {
					s.transform.ij[j][i] = *output_get++;
				}
			}
		}

		sampler_ptr->samples.push_back(s);
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

	std::vector<memory_mesh::vertex> face_vertices;
	std::vector<memory_mesh::index> face_indices;

	for (size_t index = 0; index < indices.size(); index += stride)
	{
		unsigned data_filled = 0;
		memory_mesh::vertex v;

		for (size_t offset = 0; offset < stride; offset++)
		{
			if (offset == positions.offset)
			{
				positions_source.fill_vector(v.position, indices[index + offset]);
				face_indices.push_back(indices[index + offset]);
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
			throw import_error("error importing " + filename_ + ": vertex is not filled");

		face_vertices.push_back(v);

		if (face_vertices.size() == 3) {
			for (size_t i = 0; i < 3; i++) {
				mesh_in_progress_->mesh_ptr->indices.push_back(mesh_in_progress_->getIndexForVertex(face_vertices[i], face_indices[i]));
			}

			face_vertices.clear();
		}
	}
}

}
}
