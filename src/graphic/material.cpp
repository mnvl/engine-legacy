//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
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

Material::Material(boost::shared_ptr<Renderer> const &renderer_ptr):
	renderer_(renderer_ptr)
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
		diffuse_texture_.reset(new Texture(renderer_, filename));
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
		normal_map_texture_.reset(new Texture(renderer_, filename));
	}
}

void Material::select()
{
	HRESULT hr;

	hr = renderer_->get_device()->SetTexture(0, diffuse_texture_ ? diffuse_texture_->get() : 0);
	if (FAILED(hr)) throw exception::directx_error(hr);

	hr = renderer_->get_device()->SetTexture(1, normal_map_texture_ ? normal_map_texture_->get() : 0);
	if (FAILED(hr)) throw exception::directx_error(hr);
}

void Material::bind(lua_State *L)
{
	using namespace luabind;

	module(L, "graphic")
	[
		class_<Material, boost::shared_ptr<Material> >("Material")
		.def(constructor<boost::shared_ptr<Renderer> const &>())
		.def("set_diffuse_texture", &Material::set_diffuse_texture)
		.def("set_normal_map_texture", &Material::set_normal_map_texture)
		.def("select", &Material::select)
	];
}

}

