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
#include "input.h"
#include "mouse.h"

void platform::Mouse::bind(lua_State *L)
{
	using namespace luabind;

	module_(L, "platform")
	[
		class_<Mouse, boost::shared_ptr<Mouse> >("Mouse")
		.def(constructor<boost::shared_ptr<Input> const &, bool, bool>())
		.def("update", &Mouse::update)
		.def("get_position", &Mouse::get_position)
	];
}
