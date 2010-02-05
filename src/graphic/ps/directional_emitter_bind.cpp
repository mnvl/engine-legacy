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
#include "directional_emitter.h"

namespace graphic
{
namespace ps
{

void DirectionalEmitter::bind(lua_State *L)
{
	using namespace luabind;

	module_(L, "ps")
	[
		class_<DirectionalEmitter>("Renderer")
		.def("set_size_interval", &DirectionalEmitter::set_size_interval)
		.def("set_origin", &DirectionalEmitter::set_origin)
		.def("set_direction", &DirectionalEmitter::set_direction)
		.def("set_velocity_interval", &DirectionalEmitter::set_velocity_interval)
		.def("set_acceleration_interval", &DirectionalEmitter::set_acceleration_interval)
		.def("set_ttl_interval", &DirectionalEmitter::set_ttl_interval)
	];
}

}
}
