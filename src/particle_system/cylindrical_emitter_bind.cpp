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
#include "cylindrical_emitter.h"

namespace particle_system
{

void CylindricalEmitter::bind(lua_State *L)
{
	using namespace luabind;

	module_(L, "particle_system")
	[
		class_<CylindricalEmitter>("CylindricalEmitter")
		.def_readwrite("origin", &CylindricalEmitter::origin)
		.def_readwrite("direction", &CylindricalEmitter::direction)
		.def_readwrite("size_interval", &CylindricalEmitter::size_interval)
		.def_readwrite("velocity_interval", &CylindricalEmitter::velocity_interval)
		.def_readwrite("acceleration_interval", &CylindricalEmitter::acceleration_interval)
		.def_readwrite("ttl_interval", &CylindricalEmitter::ttl_interval)
		.def_readwrite("radius_interval", &CylindricalEmitter::radius_interval)
		.def_readwrite("angle_interval", &CylindricalEmitter::angle_interval)
	];
}

}
