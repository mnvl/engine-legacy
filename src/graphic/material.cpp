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

void Material::set_diffuse_texture(std::string const &filename, size_t index)
{
	if (diffuse_textures_.size() <= index) diffuse_textures_.resize(index + 1);

	if (renderer_)
	{
		diffuse_textures_[index] = renderer_->create_texture(filename);
	}
	else
	{
		LOG_WARN("material without renderer: set_diffuse_texture() will create texture without renderer");
		diffuse_textures_[index].reset(new Texture(filename));
	}
}

void Material::set_texture_blending_weights_texture(std::string const &filename)
{
	texture_blending_weights_texture_ = renderer_->create_texture(filename);
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
}

}

