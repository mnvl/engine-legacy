//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <boost/numeric/conversion/cast.hpp>
#include "vertex_declarations.h"
#include "renderer.h"
#include "memory_model.h"
#include "memory_mesh.h"

namespace graphic {

MemoryMesh::MemoryMesh(MemoryModel *m):
	Mesh(m)
{
}

MemoryMesh::~MemoryMesh()
{
}

MeshPtr MemoryMesh::shallow_copy() const
{
	return MeshPtr(new MemoryMesh(*this));
}

void MemoryMesh::set_vertices(GenericVertex *vertices, GenericVertex::format_t format, size_t count)
{
	vertices_.resize(count);
	weights_.resize(count);

	{
		GenericVertex *vertex = vertices;

		if (initialized_vertex_components_ == 0 || format == vertex_t::FORMAT)
		{
			for (std::vector<vertex_t>::iterator it = vertices_.begin(); it != vertices_.end(); it++)
			{
				*it = *vertex++;
			}
		}
		else
		{
			for (std::vector<vertex_t>::iterator it = vertices_.begin(); it != vertices_.end(); it++)
			{
				it->assign(*vertex++, format);
			}
		}
	}

	{
		GenericVertex *vertex = vertices;

		for (std::vector<JointWeights>::iterator it = weights_.begin(); it != weights_.end(); it++)
		{
			if (format & GenericVertex::FORMAT_JOINT_INDICES) it->joints = vertex->joints;
			if (format & GenericVertex::FORMAT_JOINT_WEIGHTS) it->weights = vertex->weights;

			++vertex;
		}
	}

	initialized_vertex_components_ |= format;
}

void MemoryMesh::set_indices(unsigned *indices, size_t count)
{
	indices_.resize(count);

	for (size_t i = 0; i < count; ++i)
	{
		indices_[i] = index_t(*indices++);
	}
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
			get_model()->joints[*it]->matrix;

        joint_matrix_array.push_back(t);
    }

	result = vertices_;

	for (size_t i = 0; i < vertices_.size(); i++)
	{
		math::vec<3> pos(0, 0, 0);

		math::vec<8, unsigned> const &indices = weights_[i].joints;
		math::vec<8> const &weights = weights_[i].weights;

		for (size_t j = 0; j < 8; j++)
		{
			if (weights.i[j] > math::EPSILON)
			{
				pos += (vertices_[i].position * joint_matrix_array.at(indices.i[j])) * weights.i[j];
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
	boost::shared_ptr<Renderer> const renderer_ptr = get_model()->get_renderer();
    ShadersLibrary *shaders_library_ptr = renderer_ptr->get_shaders_library();
	IDirect3DDevice9 *device_ptr = renderer_ptr->get_device();

    device_ptr->SetVertexDeclaration(renderer_ptr->get_vertex_declarations()->get<vertex_t>());

	for (triangles_list_type::const_iterator it = triangles_list.begin();
		it != triangles_list.end(); it++)
	{
		it->material_ptr->select();

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
	boost::shared_ptr<Renderer> const renderer_ptr = get_model()->get_renderer();
    ShadersLibrary *shaders_library_ptr = renderer_ptr->get_shaders_library();
	IDirect3DDevice9 *device_ptr = renderer_ptr->get_device();

	std::vector<vertex_t> transformed_vertices;
	transform_vertices(joint_ptr, transformed_vertices);

	device_ptr->SetVertexDeclaration(renderer_ptr->get_vertex_declarations()->get<vertex_t>());

	for (triangles_list_type::const_iterator it = triangles_list.begin();
		it != triangles_list.end(); it++)
	{
		it->material_ptr->select();

		{
			math::matrix<4,4> tf = joint_ptr->matrix * renderer_ptr->get_world_matrix();
			device_ptr->SetTransform(D3DTS_WORLD, (D3DMATRIX const *) tf.ij);
		}

		device_ptr->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, it->minimal_vertex_index, it->vertices_count,
			it->triangles_count, &indices_[it->first_index_offset], INDEX_FORMAT, &transformed_vertices[0], sizeof(vertex_t));
	}
}

}
