//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2012 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <luabind/luabind.hpp>
#include "input.h"

void platform::Input::bind(lua_State *L)
{
	using namespace luabind;

	module_(L, "platform")
	[
		class_<Input, boost::shared_ptr<Input> >("Input")
		.def(constructor<boost::shared_ptr<Window> const &>())
	];
}
