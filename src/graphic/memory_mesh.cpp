
#include <boost/numeric/conversion/cast.hpp>
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

void memory_mesh::set_vertices_count(size_t count)
{
	vertices_.resize(count);
	weights_.resize(count);
}

size_t memory_mesh::get_vertices_count()
{
	return vertices_.size();
}

void memory_mesh::set_indices_count(size_t count)
{
	indices_.resize(count);
}

void memory_mesh::lock()
{
}

void memory_mesh::set_positions(math::vec<3> const *buffer)
{
	for (std::vector<vertex_t>::iterator it = vertices_.begin(); it != vertices_.end(); it++)
	{
		it->position = *buffer++;
	}
}

void memory_mesh::set_normals(math::vec<3> const *buffer)
{
	for (std::vector<vertex_t>::iterator it = vertices_.begin(); it != vertices_.end(); it++)
	{
		it->normal = *buffer++;
	}
}

void memory_mesh::set_joint_indices(math::vec<8, unsigned> const *buffer)
{
	for (std::vector<vertex_weights>::iterator it = weights_.begin(); it != weights_.end(); it++)
	{
		it->joints = *buffer++;
	}
}

void memory_mesh::set_joint_weights(math::vec<8> const *buffer)
{
	for (std::vector<vertex_weights>::iterator it = weights_.begin(); it != weights_.end(); it++)
	{
		it->factors = *buffer++;
	}
}

void memory_mesh::set_texcoords(math::vec<2> const *buffer)
{
	for (std::vector<vertex_t>::iterator it = vertices_.begin(); it != vertices_.end(); it++)
	{
		it->texcoords = *buffer++;
	}
}

void memory_mesh::set_face_indices(unsigned *buffer)
{
	for (std::vector<index_t>::iterator it = indices_.begin(); it != indices_.end(); it++)
	{
		*it = boost::numeric_cast<index_t>(*buffer++);
	}
}

void memory_mesh::unlock()
{
}

void memory_mesh::transform_vertices(joint const *joint_ptr, std::vector<vertex_t> &result) const
{
	result = vertices_;

	for (size_t i = 0; i < vertices_.size(); i++)
	{
		math::vec<3> pos(0, 0, 0);

		math::vec<8, unsigned> const &indices = weights_[i].joints;
		math::vec<8> const &factors = weights_[i].factors;

		for (size_t j = 0; j < 8; j++)
		{
			size_t joint_index = joint_index_transform[indices.i[j]];

			pos += vertices_[i].position *
				bind_shape_matrix *
				inverted_bind_matrices[joint_index] *
				model_ptr_->joints[joint_index].matrix *
				factors.i[j];
		}

		result[i].position = pos;
	}
}

void memory_mesh::draw_static(joint *joint_ptr) const
{
    renderer *renderer_ptr = get_model()->get_renderer();
    resource_manager *resource_manager_ptr = renderer_ptr->get_resource_manager();
	IDirect3DDevice9 *device_ptr = renderer_ptr->get_device();

    device_ptr->SetVertexDeclaration(resource_manager_ptr->get_vertex_decl<vertex_t>());

	for (triangles_list_type::const_iterator it = triangles_list.begin();
		it != triangles_list.end(); it++)
	{

		if (it->material_ptr)
		{
			it->material_ptr->select();
		}
		else
		{
			resource_manager_ptr->get_default_material()->select();
		}

		device_ptr->SetTransform(D3DTS_WORLDMATRIX(0), (D3DMATRIX const *) joint_ptr->matrix.ij);

		device_ptr->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, it->minimal_vertex_index, it->vertices_count,
			it->triangles_count, &indices_[it->first_index_offset], D3DFMT_INDEX32, &vertices_[0], sizeof(vertex_t));
	}
}

void memory_mesh::draw_animated(joint *joint_ptr) const
{
    renderer *renderer_ptr = get_model()->get_renderer();
    resource_manager *resource_manager_ptr = renderer_ptr->get_resource_manager();
	IDirect3DDevice9 *device_ptr = renderer_ptr->get_device();

	std::vector<vertex_t> transformed_vertices;
	transform_vertices(joint_ptr, transformed_vertices);

	device_ptr->SetVertexDeclaration(resource_manager_ptr->get_vertex_decl<vertex_t>());

	for (triangles_list_type::const_iterator it = triangles_list.begin();
		it != triangles_list.end(); it++)
	{
		if (it->material_ptr)
		{
			it->material_ptr->select();
		}
		else
		{
			resource_manager_ptr->get_default_material()->select();
		}

		device_ptr->SetTransform(D3DTS_WORLDMATRIX(0), (D3DMATRIX const *) joint_ptr->matrix.ij);

		device_ptr->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, it->minimal_vertex_index, it->vertices_count,
			it->triangles_count, &indices_[it->first_index_offset], D3DFMT_INDEX32, &transformed_vertices[0], sizeof(vertex_t));
	}
}

}
