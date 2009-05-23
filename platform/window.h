#pragma once

#include <windows.h>
#include <util/luna.h>

namespace platform {

class window {
public:
	LUNA_CLASS(window);

	window(lua_State *L);

	window(char const *title = 0);
	~window();

	HWND get_handle() const throw() { return handle_; }

	void create(char const *title);
	int run_message_loop(lua_State *L);
	LRESULT process_message(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	HWND handle_;

	static LRESULT CALLBACK dispatch_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

}
