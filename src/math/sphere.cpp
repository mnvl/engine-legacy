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
#include "sphere.h"

namespace math
{

void bind_sphere(lua_State *L)
{
	using namespace luabind;

	module(L, "math")
	[
		class_<sphere<3> >("sphere3")
		.def_readwrite("centre", &sphere<3>::centre)
		.def_readwrite("radius", &sphere<3>::radius)
	];
}

}
