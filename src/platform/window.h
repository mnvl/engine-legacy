#pragma once

#include <windows.h>
#include <lua.h>
#include <math/vec.h>

namespace platform {

class Window {
public:
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
