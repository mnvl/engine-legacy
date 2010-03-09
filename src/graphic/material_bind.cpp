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
#include "material.h"

namespace graphic
{

void Material::bind(lua_State *L)
{
	using namespace luabind;

	module(L, "graphic")
	[
		class_<Material, boost::shared_ptr<Material> >("Material")
		.def(constructor<boost::shared_ptr<Renderer> const &>())
		.def("get_name", &Material::get_name)
		.def("set_diffuse_texture", &Material::set_diffuse_texture)
		.def("set_normal_map_texture", &Material::set_normal_map_texture)
		.def("select", &Material::select)
	];
}

}

