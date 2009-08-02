#pragma once

#include <windows.h>

namespace platform {

class Window {
public:
	Window(char const *title = 0);
	~Window();

	HWND get_handle() const throw() { return handle_; }

	void create(char const *title);
	bool run_message_loop();
	LRESULT process_message(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	HWND handle_;

	static LRESULT CALLBACK dispatch_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

}
