#pragma once
//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <windows.h>
#include <boost/signals2/signal.hpp>
#include <luabind/lua_include.hpp>
#include <math/vec.h>

namespace platform
{

class Window {
public:
	boost::signals2::signal0<void> signal_resize;

	Window(char const *title);
	~Window();

	HWND get_handle() const throw() { return handle_; }
	math::vec<2> const &get_mouse_position() const throw() { return mouse_position_; }
	math::vec<3, bool> const &get_mouse_buttons() const throw() { return mouse_buttons_; }

	bool run_message_loop();
	LRESULT process_message(UINT uMsg, WPARAM wParam, LPARAM lParam);

	static void bind(lua_State *L);

private:
	HWND handle_;
	math::vec<2> mouse_position_;
	math::vec<3, bool> mouse_buttons_;

	static LRESULT CALLBACK dispatch_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

}
