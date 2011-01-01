//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2011 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <luabind/luabind.hpp>
#include <exception/directx_error.h>
#include <util/logger.h>
#include "material.h"

namespace graphic
{

Material::Material(RendererPtr const &renderer_ptr):
	renderer_(renderer_ptr),
	cull_mode_(CULL_NONE),
	zwrite_enabled_(true),
	alpha_blend_enabled_(false),
	src_blend_mode_(BLEND_ONE),
	dest_blend_mode_(BLEND_ZERO)
{
}

Material::~Material()
{
}

void Material::set_diffuse_texture(std::string const &filename)
{
	if (renderer_)
	{
		diffuse_texture_ = renderer_->create_texture(filename);
	}
	else
	{
		LOG_WARN("material without renderer: set_diffuse_texture() will create texture without renderer");
		diffuse_texture_.reset(new Texture(filename));
	}
}

void Material::set_normal_map_texture(std::string const &filename)
{
	if (renderer_)
	{
		normal_map_texture_ = renderer_->create_texture(filename);
	}
	else
	{
		LOG_WARN("material without renderer: set_normal_map_texture() will create texture without renderer");
		normal_map_texture_.reset(new Texture(filename));
	}
}

void Material::select()
{
	static DWORD cull_mode_table[] = { D3DCULL_CW, D3DCULL_CCW, D3DCULL_NONE };
	static DWORD blend_mode_table[] = { D3DBLEND_ZERO, D3DBLEND_ONE };

	HRESULT hr = 0;

	hr = renderer_->get_device()->SetRenderState(D3DRS_CULLMODE, cull_mode_table[cull_mode_]);
	if (FAILED(hr)) throw exception::directx_error(hr);

	hr = renderer_->get_device()->SetRenderState(D3DRS_ZWRITEENABLE, zwrite_enabled_);
	if (FAILED(hr)) throw exception::directx_error(hr);

	hr = renderer_->get_device()->SetRenderState(D3DRS_SRCBLEND, blend_mode_table[src_blend_mode_]);
	if (FAILED(hr)) throw exception::directx_error(hr);

	hr = renderer_->get_device()->SetRenderState(D3DRS_DESTBLEND, blend_mode_table[dest_blend_mode_]);
	if (FAILED(hr)) throw exception::directx_error(hr);

	hr = renderer_->get_device()->SetRenderState(D3DRS_ALPHABLENDENABLE, alpha_blend_enabled_);
	if (FAILED(hr)) throw exception::directx_error(hr);
}

}

