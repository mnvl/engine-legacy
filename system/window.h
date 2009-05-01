#pragma once

#include <windows.h>

namespace engine {
namespace system {

class window {
public:
	window(char const *title = 0);
	~window();

	HWND get_handle() const throw() { return handle_; }

	void create(char const *title);
	void run_message_loop();
	LRESULT process_message(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	HWND handle_;

	static LRESULT CALLBACK dispatch_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

}
}