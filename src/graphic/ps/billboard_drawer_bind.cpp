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
#include "billboard_drawer.h"

namespace graphic
{
namespace ps
{

void BillboardDrawer::bind(lua_State *L)
{
	using namespace luabind;

	module(L, "ps")
	[
		class_<BillboardDrawer>("BillboardDrawer")
		.def("set_material", &BillboardDrawer::set_material)
	];
}

}
}
