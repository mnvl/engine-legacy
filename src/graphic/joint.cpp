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
#include "joint.h"

namespace graphic
{

void Joint::bind(lua_State *L)
{
	using namespace luabind;

	module_(L, "graphic")
	[
		class_<Joint, JointPtr>("Joint")
		.def_readwrite("matrix", &Joint::matrix)
	];
}

}
