
#include <engine/graphic/renderer.h>
#include <engine/graphic/memory_model.h>
#include <engine/graphic/memory_mesh.h>

namespace engine {
namespace graphic {

memory_mesh::memory_mesh(memory_model *model_ptr):
model_ptr_(model_ptr)
{
}

memory_mesh::~memory_mesh() {
}

model::joint_influence *memory_mesh::get_joint_influence_by_name(std::string const &name, bool can_create)
{
	model::joint *joint_ptr = model_ptr_->get_joint_by_name(name);

	for (influences_type::iterator it = influences.begin(); it != influences.end(); it++) {
		if (it->joint_ptr == joint_ptr) return &*it;
	}

	if (can_create) {
		joint_influence dummy;
		dummy.joint_ptr = joint_ptr;
		influences.push_back(dummy);
		return &influences.back();
	}

	throw std::runtime_error("influence of joint " + joint_ptr->name + " to mesh " + name + " not found");
}

void memory_mesh::draw_wireframe(renderer *renderer_ptr, math::scalar time) const
{
	std::vector<vertex> animated_vertices;
	animated_vertices = vertices;

	if (time > 0)
	{
		for (size_t i = 0; i < animated_vertices.size(); i++) {
			animated_vertices[i].position.set(0, 0, 0);
		}

		for (influences_type::const_iterator it = influences.begin(); it != influences.end(); it++)
		{
			for (joint_influence::vertex_weights_type::const_iterator jt = it->vertex_weights.begin(); jt != it->vertex_weights.end(); jt++)
			{
				animated_vertices[jt->vertex_index].position += 
					vertices[jt->vertex_index].position *
					transform *
					it->inverted_bind_matrix *
					it->joint_ptr->interpolated_transform *
					jt->weight;
			}
		}
	}
	else
	{
		for (size_t i = 0; i < animated_vertices.size(); i++) {
			animated_vertices[i].position = vertices[i].position;
		}

	}

	size_t triangles_count = indices.size() / 3;

	std::vector<index> dummy;
	dummy.reserve(6 * triangles_count);

	for (size_t i = 0; i < triangles_count; i++) {
		dummy.push_back(indices[3 * i + 0]);
		dummy.push_back(indices[3 * i + 1]);

		dummy.push_back(indices[3 * i + 1]);
		dummy.push_back(indices[3 * i + 2]);

		dummy.push_back(indices[3 * i + 2]);
		dummy.push_back(indices[3 * i + 0]);
	}

	renderer_ptr->get_device()->DrawIndexedPrimitiveUP(D3DPT_LINELIST, 0, animated_vertices.size(),
		dummy.size() / 2, &dummy[0], INDEX_FORMAT, &animated_vertices[0], sizeof(vertex));
}

}
}