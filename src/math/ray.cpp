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
#include "ray.h"

namespace math
{

void bind_ray(lua_State *L)
{
	using namespace luabind;

	module(L, "math")
	[
		class_<ray<2> >("ray2")
		.def(constructor<>())
		.def(constructor<const vec<2> &, const vec<2> &>())
		.def("set", &ray<2>::set)
		.def("apply", &ray<2>::apply)
		.def_readwrite("origin", &ray<2>::r0)
		.def_readwrite("direction", &ray<2>::a),

		class_<ray<3> >("ray3")
		.def(constructor<>())
		.def(constructor<const vec<3> &, const vec<3> &>())
		.def("set", &ray<3>::set)
		.def("apply", &ray<3>::apply)
		.def_readwrite("r0", &ray<3>::r0)
		.def_readwrite("a", &ray<3>::a)
	];
}

}
