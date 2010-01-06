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
#include "plane.h"

namespace math
{

void bind_plane(lua_State *L)
{
	using namespace luabind;

	module(L, "math")
	[
		class_<plane<> >("plane")
		.def(constructor<>())
		.def(constructor<const vec<3> &,const vec<3> &>())
		.def("trace", (scalar (plane<>::*)(const ray<3> &) const) &plane<>::trace)
	];
}

}
