//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2011 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <exception/directx_error.h>
#include "vertex_array.h"
#include "index_array.h"
#include "material.h"
#include "uber_shader.h"
#include "d3d9_vertex_buffer.h"
#include "d3d9_index_buffer.h"
#include "d3d9_vertex_declarations.h"
#include "d3d9_shader.h"
#include "d3d9_renderer.h"
#include "d3d9_mesh_draw_call.h"

namespace graphic
{

D3D9MeshDrawCall::D3D9MeshDrawCall(D3D9RendererPtr const &renderer):
	MeshDrawCall(renderer),
	renderer_(renderer)
{
}

D3D9MeshDrawCall::~D3D9MeshDrawCall()
{
}

void D3D9MeshDrawCall::execute()
{
	HRESULT hr;

	IDirect3DDevice9 *device_ptr = renderer_->get_device();
	D3D9VertexBuffer *vertex_buffer = static_cast<D3D9VertexBuffer *>(vertex_buffer_.get());
	D3D9IndexBuffer *index_buffer = static_cast<D3D9IndexBuffer *>(index_buffer_.get());
	VertexTraits const *vertex_traits = vertex_buffer->get_vertex_traits();
	D3D9Shader *shader = static_cast<D3D9Shader *>(shader_->get_shader().get());

	hr = device_ptr->SetVertexDeclaration(renderer_->get_vertex_declarations()->get(vertex_traits));
	if (FAILED(hr)) throw exception::directx_error(hr);

    hr = device_ptr->SetStreamSource(0, vertex_buffer->get(), 0, vertex_traits->get_size());
	if (FAILED(hr)) throw exception::directx_error(hr);

    hr = device_ptr->SetIndices(index_buffer->get());
	if (FAILED(hr)) throw exception::directx_error(hr);

	if (material_) material_->select();

	shader_->get_technique_for_vertex(vertex_traits)->set();

	shader_->diffuse_texture->set(material_ ? material_->get_diffuse_texture() : TexturePtr());
	shader_->normal_map_texture->set(material_ ? material_->get_normal_map_texture() : TexturePtr());

	shader_->world_matrix->set(world_matrix_);
	shader_->world_view_projection_matrix->set(world_view_projection_matrix_);

	if (joint_matrix_array_.size()) shader_->joint_matrix_array->set(joint_matrix_array_);

	UINT passes_count;
	hr = shader->get()->Begin(&passes_count, D3DXFX_DONOTSAVESTATE);
	if (FAILED(hr)) throw exception::directx_error(hr);

	for(UINT pass = 0; pass < passes_count; pass++)
	{
		hr = shader->get()->BeginPass(pass);
		if (FAILED(hr)) throw exception::directx_error(hr);

		hr = device_ptr->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, minimal_vertex_index_, vertices_count_,
			first_index_offset_, triangles_count_);
		if (FAILED(hr)) throw exception::directx_error(hr);

		hr = shader->get()->EndPass();
		if (FAILED(hr)) throw exception::directx_error(hr);
	}

	hr = shader->get()->End();
	if (FAILED(hr)) throw exception::directx_error(hr);
}

}
