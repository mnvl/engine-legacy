//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <d3d9.h>
#include <vector>
#include "mesh_shader.h"
#include "animated_model.h"
#include "renderer.h"
#include "animated_mesh.h"

namespace graphic
{

AnimatedMesh::AnimatedMesh(Model *model_ptr):
    PnjwtStaticMesh(model_ptr)
{
}

MeshPtr AnimatedMesh::shallow_copy() const
{
	return MeshPtr(new AnimatedMesh(*this));
}

AnimatedModel *AnimatedMesh::get_model()
{
    return (AnimatedModel *) Mesh::get_model();
}

void AnimatedMesh::draw(Joint *joint_ptr)
{
    HRESULT hr;

	boost::shared_ptr<Renderer> const renderer_ptr = get_model()->get_renderer();
	SharedResources * const shared_resources_ptr = renderer_ptr->get_shared_resources();
	IDirect3DDevice9 * const device_ptr = renderer_ptr->get_device();

	math::matrix<4,4> world_matrix = joint_ptr->matrix *
        renderer_ptr->get_world_matrix();

	math::matrix<4,4> world_view_projection_matrix =
		world_matrix *
        renderer_ptr->get_view_matrix() *
        renderer_ptr->get_projection_matrix();

	shader_->world_view_projection_matrix->set(world_view_projection_matrix);
	shader_->world_matrix->set(world_matrix);

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

	if (joint_matrix_array.empty())
	{
	    hr = shader_->get()->SetMatrixArray("joint_matrix_array", (D3DXMATRIX const *) 0, 0);
	}
	else
	{
	    hr = shader_->get()->SetMatrixArray("joint_matrix_array", (D3DXMATRIX const *) &joint_matrix_array[0], joint_matrix_array.size());
	}

	if (FAILED(hr)) throw exception::directx_error(hr);

	hr = device_ptr->SetVertexDeclaration(shared_resources_ptr->get_vertex_decl<vertex_t>());
	if (FAILED(hr)) throw exception::directx_error(hr);

    hr = device_ptr->SetStreamSource(0, vertex_buffer_->get(), 0, sizeof(vertex_t));
	if (FAILED(hr)) throw exception::directx_error(hr);

    hr = device_ptr->SetIndices(index_buffer_->get());
	if (FAILED(hr)) throw exception::directx_error(hr);

	// TODO:
	//draw_triangles_list();
}

}
