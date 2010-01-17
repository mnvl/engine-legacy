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
#include "aabb.h"

namespace math
{

void bind_aabb(lua_State *L)
{
	using namespace luabind;

	module(L, "math")
	[
		class_<aabb<3> >("aabb3")
		.def(constructor<math::vec<3> const &, math::vec<3> const &>())
		.def_readwrite("lo", &aabb<3>::lo)
		.def_readwrite("hi", &aabb<3>::hi)
	];
}

}
