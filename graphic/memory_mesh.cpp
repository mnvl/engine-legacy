
#include <graphic/renderer.h>
#include <graphic/memory_model.h>
#include <graphic/memory_mesh.h>

namespace graphic {

memory_mesh::memory_mesh(memory_model *m):
mesh(m)
{
}

memory_mesh::~memory_mesh() {
}

void memory_mesh::transform_vertices(joint const *joint_ptr, std::vector<vertex_t> &result) const
{
	result = vertices;

	if (get_model()->animations.empty())
	{
		for (std::vector<vertex_t>::iterator it = result.begin(); it != result.end(); it++)
		{
			it->position = it->position * joint_ptr->matrix;
		}

		return;
	}

	for (size_t i = 0; i < vertices.size(); i++)
	{
		result[i].position.set(0, 0, 0);
	}

	std::vector<math::scalar> factor(vertices.size(), 0.0f);

	for (influences_type::const_iterator it = influences.begin(); it != influences.end(); it++)
	{
		for (joint_influence::vertex_weights_type::const_iterator jt = it->vertex_weights.begin(); jt != it->vertex_weights.end(); jt++)
		{
			factor[jt->vertex_index] += jt->weight;
		}
	}

	for (influences_type::const_iterator it = influences.begin(); it != influences.end(); it++)
	{
		for (joint_influence::vertex_weights_type::const_iterator jt = it->vertex_weights.begin(); jt != it->vertex_weights.end(); jt++)
		{
			result[jt->vertex_index].position +=
				((vertices[jt->vertex_index].position * bind_shape_matrix) *
				it->inverted_bind_matrix * it->joint_ptr->matrix) *
				jt->weight;
		}
	}
}

void memory_mesh::draw_static(joint *joint_ptr) const
{
}

}
