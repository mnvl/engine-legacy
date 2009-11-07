
#include <boost/numeric/conversion/cast.hpp>
#include <graphic/renderer.h>
#include <graphic/memory_model.h>
#include <graphic/memory_mesh.h>

namespace graphic {

MemoryMesh::MemoryMesh(MemoryModel *m):
Mesh(m)
{
}

MemoryMesh::~MemoryMesh()
{
}

void MemoryMesh::set_vertices_count(size_t count)
{
	vertices_.resize(count);
	weights_.resize(count);
}

size_t MemoryMesh::get_vertices_count()
{
	return vertices_.size();
}

void MemoryMesh::set_indices_count(size_t count)
{
	indices_.resize(count);
}

void MemoryMesh::lock()
{
}

void MemoryMesh::set_positions(math::vec<3> const *buffer)
{
	for (std::vector<vertex_t>::iterator it = vertices_.begin(); it != vertices_.end(); it++)
	{
		it->position = *buffer++;
	}
}

void MemoryMesh::set_normals(math::vec<3> const *buffer)
{
	for (std::vector<vertex_t>::iterator it = vertices_.begin(); it != vertices_.end(); it++)
	{
		it->normal = *buffer++;
	}
}

void MemoryMesh::set_joint_indices(math::vec<8, unsigned> const *buffer)
{
	for (std::vector<vertex_weights>::iterator it = weights_.begin(); it != weights_.end(); it++)
	{
		it->joints = *buffer++;
	}
}

void MemoryMesh::set_joint_weights(math::vec<8> const *buffer)
{
	for (std::vector<vertex_weights>::iterator it = weights_.begin(); it != weights_.end(); it++)
	{
		it->factors = *buffer++;
	}
}

void MemoryMesh::set_texcoords(math::vec<2> const *buffer)
{
	for (std::vector<vertex_t>::iterator it = vertices_.begin(); it != vertices_.end(); it++)
	{
		it->texcoords = *buffer++;
	}
}

void MemoryMesh::set_face_indices(unsigned *buffer)
{
	for (std::vector<index_t>::iterator it = indices_.begin(); it != indices_.end(); it++)
	{
		*it = boost::numeric_cast<index_t>(*buffer++);
	}
}

void MemoryMesh::unlock()
{
}

void MemoryMesh::transform_vertices(Joint const *joint_ptr, std::vector<vertex_t> &result) const
{
    std::vector<math::matrix<4,4> > joint_matrix_array;
    joint_matrix_array.reserve(joint_index_transform.size());

    for (joint_index_transform_type::const_iterator it = joint_index_transform.begin();
         it != joint_index_transform.end(); it++)
    {
		math::matrix<4,4> t = bind_shape_matrix *
			inverted_bind_matrices[*it] *
			get_model()->joints[*it].matrix;

        joint_matrix_array.push_back(t);
    }

	result = vertices_;

	for (size_t i = 0; i < vertices_.size(); i++)
	{
		math::vec<3> pos(0, 0, 0);

		math::vec<8, unsigned> const &indices = weights_[i].joints;
		math::vec<8> const &factors = weights_[i].factors;

		for (size_t j = 0; j < 8; j++)
		{
			if (factors.i[j] > math::EPSILON)
			{
				pos += (vertices_[i].position * joint_matrix_array.at(indices.i[j])) * factors.i[j];
			}
		}

		result[i].position = pos;
	}
}

// Results of rendering via MemoryMesh and AnimatedMesh may differ if model have joint weights
// and no animation: AnimatedMesh always computes positions of vertices using weights but MemoryMesh
// uses weights only if model have animation. You can easily check if this is the cause just replacing
// AnimatedMesh with StaticMesh and checking that rendering result is matching the one of MemoryMesh.
void MemoryMesh::draw_static(Joint *joint_ptr) const
{
    Renderer *renderer_ptr = get_model()->get_renderer();
    ResourceManager *resource_manager_ptr = renderer_ptr->get_resource_manager();
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

		{
			math::matrix<4,4> tf = joint_ptr->matrix * renderer_ptr->get_world_matrix();
			device_ptr->SetTransform(D3DTS_WORLD, (D3DMATRIX const *) tf.ij);
		}

		device_ptr->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, it->minimal_vertex_index, it->vertices_count,
			it->triangles_count, &indices_[it->first_index_offset], INDEX_FORMAT, &vertices_[0], sizeof(vertex_t));
	}
}

void MemoryMesh::draw_animated(Joint *joint_ptr) const
{
    Renderer *renderer_ptr = get_model()->get_renderer();
    ResourceManager *resource_manager_ptr = renderer_ptr->get_resource_manager();
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

		{
			math::matrix<4,4> tf = joint_ptr->matrix * renderer_ptr->get_world_matrix();
			device_ptr->SetTransform(D3DTS_WORLD, (D3DMATRIX const *) tf.ij);
		}

		device_ptr->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, it->minimal_vertex_index, it->vertices_count,
			it->triangles_count, &indices_[it->first_index_offset], INDEX_FORMAT, &transformed_vertices[0], sizeof(vertex_t));
	}
}

}
