
#include <graphic/renderer.h>
#include <graphic/memory_model.h>
#include <graphic/memory_mesh.h>

namespace graphic {

memory_mesh::memory_mesh(memory_model *m):
mesh(m)
{
}

memory_mesh::~memory_mesh()
{
}

void memory_mesh::set_vertex_buffer(vertex_t *vertices, size_t count)
{
	vertices_.assign(vertices, vertices + count);
}

void memory_mesh::set_index_buffer(index_t *indices, size_t count)
{
	indices_.assign(indices, indices + count);
}

void memory_mesh::transform_vertices(joint const *joint_ptr, std::vector<vertex_t> &result) const
{
	result = vertices_;

	if (get_model()->animations.empty())
	{
		for (std::vector<vertex_t>::iterator it = result.begin(); it != result.end(); it++)
		{
			it->position = it->position * joint_ptr->matrix;
		}

		return;
	}

	for (size_t i = 0; i < vertices_.size(); i++)
	{
		result[i].position.set(0, 0, 0);
	}

	for (influences_type::const_iterator it = influences.begin(); it != influences.end(); it++)
	{
		for (joint_influence::vertex_weights_type::const_iterator jt = it->vertex_weights.begin(); jt != it->vertex_weights.end(); jt++)
		{
			result[jt->vertex_index].position +=
				((vertices_[jt->vertex_index].position * bind_shape_matrix) *
				it->inverted_bind_matrix * it->joint_ptr->matrix) *
				jt->weight;
		}
	}
}

void memory_mesh::draw_static(joint *joint_ptr) const
{
	IDirect3DDevice9 *device_ptr = get_model()->get_renderer()->get_device();

	for (triangles_list_type::const_iterator it = triangles_list.begin();
		it != triangles_list.end(); it++)
	{
		device_ptr->SetFVF(VERTEX_FORMAT);

		if (it->material_ptr)
		{
			it->material_ptr->select();
		}
		else
		{
			get_model()->get_renderer()->get_default_material()->select();
		}

		device_ptr->SetTransform(D3DTS_WORLDMATRIX(0), (D3DMATRIX const *) joint_ptr->matrix.ij);

		device_ptr->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, it->minimal_vertex_index, it->vertices_count,
			it->triangles_count, &indices_[it->first_index_offset], D3DFMT_INDEX16, &vertices_[0], sizeof(vertex_t));
	}
}

void memory_mesh::draw_animated(joint *joint_ptr) const
{
	IDirect3DDevice9 *device_ptr = get_model()->get_renderer()->get_device();

	std::vector<vertex_t> transformed_vertices;
	transform_vertices(joint_ptr, transformed_vertices);

	for (triangles_list_type::const_iterator it = triangles_list.begin();
		it != triangles_list.end(); it++)
	{
		device_ptr->SetFVF(VERTEX_FORMAT);

		if (it->material_ptr)
		{
			it->material_ptr->select();
		}
		else
		{
			get_model()->get_renderer()->get_default_material()->select();
		}

		device_ptr->SetTransform(D3DTS_WORLDMATRIX(0), (D3DMATRIX const *) joint_ptr->matrix.ij);

		device_ptr->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, it->minimal_vertex_index, it->vertices_count,
			it->triangles_count, &indices_[it->first_index_offset], D3DFMT_INDEX16, &transformed_vertices[0], sizeof(vertex_t));
	}
}

}
