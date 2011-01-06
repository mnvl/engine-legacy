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
#include "material.h"

namespace graphic
{

void Material::bind(lua_State *L)
{
	using namespace luabind;

	module(L, "graphic")
	[
		class_<Material, MaterialPtr >("Material")
		.enum_("blend_mode")
		[
			value("BLEND_ZERO", BLEND_ZERO),
			value("BLEND_ONE", BLEND_ONE),
			value("BLEND_SRC_ALPHA", BLEND_SRC_ALPHA),
			value("BLEND_INV_SRC_ALPHA", BLEND_INV_SRC_ALPHA),
			value("BLEND_DEST_ALPHA", BLEND_DEST_ALPHA),
			value("BLEND_INV_DEST_ALPHA", BLEND_INV_DEST_ALPHA)
		]
		.def(constructor<RendererPtr const &>())
		.def("get_name", &Material::get_name)
		.def("set_alpha_blend_enabled", &Material::set_alpha_blend_enabled)
		.def("set_src_blend_mode", &Material::set_src_blend_mode)
		.def("set_dest_blend_mode", &Material::set_dest_blend_mode)
		.def("set_diffuse_texture", &Material::set_diffuse_texture)
		.def("set_normal_map_texture", &Material::set_normal_map_texture)
		.def("set_normal_map_texture", &Material::set_normal_map_texture)
		.def("select", &Material::select)
	];
}

}

