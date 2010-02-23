//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <math/frustum.h>
#include "vertex_traits.h"
#include "vertex_array.h"
#include "index_array.h"
#include "camera.h"
#include "shaders_library.h"
#include "vertex_declarations.h"
#include "texture.h"
#include "material.h"
#include "renderer.h"
#include "draw_call.h"

namespace graphic
{

DrawCall::DrawCall(boost::shared_ptr<Renderer> const &renderer):
	renderer_(renderer),
	minimal_vertex_index_(0),
	vertices_count_(0),
	first_index_offset_(0),
	triangles_count_(0)
{
	world_matrix_.identity();
	view_matrix_ = renderer->get_view_matrix();
	projection_matrix_ = renderer->get_projection_matrix();
}

DrawCall::~DrawCall()
{
}

void DrawCall::set_obb(math::obb<3> const &bounds)
{
	bounds_ = bounds;
}

void DrawCall::set_vertex_buffer(boost::shared_ptr<VertexBuffer> const &vertex_buffer)
{
	vertex_buffer_ = vertex_buffer;
}

void DrawCall::set_index_buffer(boost::shared_ptr<IndexBuffer> const &index_buffer)
{
	index_buffer_ = index_buffer;
}

void DrawCall::set_material(boost::shared_ptr<Material> const &material)
{
	material_ = material;
}

void DrawCall::set_minimal_vertex_index(size_t minimal_vertex_index)
{
	minimal_vertex_index_ = minimal_vertex_index;
}

void DrawCall::set_vertices_count(size_t vertices_count)
{
	vertices_count_ = vertices_count;
}

void DrawCall::set_first_index_offset(size_t first_index_offset)
{
	first_index_offset_ = first_index_offset;
}

void DrawCall::set_triangles_count(size_t triangles_count)
{
	triangles_count_ = triangles_count;
}

void DrawCall::set_world_matrix(math::matrix<4,4> const &world_matrix)
{
	world_matrix_ = world_matrix;
}

void DrawCall::set_camera(boost::shared_ptr<Camera> const &camera)
{
	view_matrix_ = camera->get_view_matrix();
	projection_matrix_ = camera->get_projection_matrix();
}

void DrawCall::commit()
{
	if (!vertex_buffer_) throw std::logic_error("vertex buffer not set");
	if (!material_) throw std::logic_error("material not set");

	world_view_projection_matrix_ = world_matrix_ * view_matrix_ * projection_matrix_;
}

bool DrawCall::is_visible() const
{
	return math::frustum<>(world_view_projection_matrix_).test_intersection(bounds_);
}

void DrawCall::execute()
{
	HRESULT hr;

    ShadersLibrary *shaders_library_ptr = renderer_->get_shaders_library();
	IDirect3DDevice9 *device_ptr = renderer_->get_device();

	VertexTraits const *vertex_traits = vertex_buffer_->get_vertex_traits();

	hr = device_ptr->SetVertexShader(0);
	if (FAILED(hr)) throw exception::directx_error(hr);

	hr = device_ptr->SetPixelShader(0);
	if (FAILED(hr)) throw exception::directx_error(hr);

	hr = device_ptr->SetVertexDeclaration(renderer_->get_vertex_declarations()->get(vertex_traits));
	if (FAILED(hr)) throw exception::directx_error(hr);

    hr = device_ptr->SetStreamSource(0, vertex_buffer_->get(), 0, vertex_traits->get_size());
	if (FAILED(hr)) throw exception::directx_error(hr);

    hr = device_ptr->SetIndices(index_buffer_->get());
	if (FAILED(hr)) throw exception::directx_error(hr);

	hr = device_ptr->SetTexture(0, material_ ? material_->get_diffuse_texture()->get() : 0);
	if (FAILED(hr)) throw exception::directx_error(hr);
}

}
