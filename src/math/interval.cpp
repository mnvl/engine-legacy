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
#include "interval.h"

namespace math
{

void bind_interval(lua_State *L)
{
	using namespace luabind;

	module(L, "math")
	[
		class_<interval<> >("interval")
		.def(constructor<>())
		.def(constructor<scalar, scalar>())
		.def("length", &interval<>::length)
		.def("random", &interval<>::random)
	];
}

}
