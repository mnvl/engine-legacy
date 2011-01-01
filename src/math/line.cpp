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
#include "line.h"

namespace math
{

void bind_line(lua_State *L)
{
	using namespace luabind;

	module(L, "math")
	[
		class_<line<3> >("line3")
		.def_readwrite("A", &line<3>::A)
		.def_readwrite("B", &line<3>::B)
	];
}

}
