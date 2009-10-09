#pragma once

#include <windows.h>
#include <luabind/lua_include.hpp>

namespace platform {

class Window {
public:
	Window();
	~Window();

	HWND get_handle() const throw() { return handle_; }

	void create(char const *title);
	bool run_message_loop();
	LRESULT process_message(UINT uMsg, WPARAM wParam, LPARAM lParam);

	static void bind(lua_State *L);

private:
	HWND handle_;

	static LRESULT CALLBACK dispatch_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

}
