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
		.def_readwrite("origin", &DirectionalEmitter::origin)
		.def_readwrite("direction", &DirectionalEmitter::direction)
		.def_readwrite("size_interval", &DirectionalEmitter::size_interval)
		.def_readwrite("velocity_interval", &DirectionalEmitter::velocity_interval)
		.def_readwrite("acceleration_interval", &DirectionalEmitter::acceleration_interval)
		.def_readwrite("ttl_interval", &DirectionalEmitter::ttl_interval)
	];
}

}
}
