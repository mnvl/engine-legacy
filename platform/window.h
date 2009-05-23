#pragma once

#include <windows.h>

namespace platform {

class window {
public:
    static char const CLASS_NAME[];

    /*
	window(lua_State *L);
	*/

	window(char const *title = 0);
	~window();

	HWND get_handle() const throw() { return handle_; }

	void create(char const *title);
	bool run_message_loop();
	LRESULT process_message(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	HWND handle_;

	static LRESULT CALLBACK dispatch_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

}
