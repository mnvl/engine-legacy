
#include <engine/graphic/renderer.h>
#include <engine/graphic/memory_mesh.h>

namespace engine {
namespace graphic {

memory_mesh::memory_mesh() {
}

memory_mesh::~memory_mesh() {
}

void memory_mesh::draw_wireframe(renderer *renderer_ptr, math::scalar time) const
{
	std::vector<vertex> animated_vertices;
	animated_vertices = vertices;

	if (time > 0)
	{
		for (influences_type::const_iterator it = influences.begin(); it != influences.end(); it++)
		{
			for (joint_influence::vertex_weights_type::const_iterator jt = it->vertex_weights.begin(); jt != it->vertex_weights.end(); jt++)
			{
				if (it->joint_ptr) {
					animated_vertices[jt->vertex_index].position += animated_vertices[jt->vertex_index].position * it->joint_ptr->frame_transform * jt->weight;
				}
			}
		}
	}

	size_t triangles_count = indices.size() / 3;

	std::vector<index> dummy;
	dummy.reserve(4 * 2 * triangles_count);

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