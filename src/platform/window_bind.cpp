//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2011 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <boost/shared_ptr.hpp>
#include <luabind/luabind.hpp>
#include <luabind/raw_policy.hpp>
#include "window.h"

void platform::Window::bind(lua_State *L)
{
	using namespace luabind;

    module_(L, "platform")
    [
        class_<Window, boost::shared_ptr<Window> >("Window")
		.def(constructor<char const *>())
		.def(constructor<lua_State *>())
		.def("run_message_loop", &Window::run_message_loop)
		.def("get_mouse_position", &Window::get_mouse_position)
		.def("get_mouse_buttons", &Window::get_mouse_buttons)
    ];
}
