
#include <stdio.h>
#include <exception/winapi_error.h>
#include <platform/window.h>

namespace platform
{

Window::Window():
	handle_(0)
{
}

Window::~Window() {
	if (handle_) {
		DestroyWindow(handle_);
		handle_ = 0;
	}
}

void Window::create(char const *title) {
	char class_name[sizeof(Window *)*2+1];
	sprintf(class_name,"%p",this);

	WNDCLASSEX wc;
	wc.cbSize = sizeof(wc);
	wc.style = 0;
	wc.lpfnWndProc  =  dispatch_message;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(0);
	wc.hIcon  =  0;
	wc.hCursor = GetCursor();
	wc.hbrBackground = 0;
	wc.lpszMenuName = 0;
	wc.lpszClassName = class_name;
	wc.hIconSm = 0;
	if(!RegisterClassEx(&wc)) throw exception::winapi_error();

	CreateWindowEx(0, wc.lpszClassName, title, WS_OVERLAPPED|WS_VISIBLE|WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		0, 0, wc.hInstance, 0);
	if(!handle_) throw exception::winapi_error();
}

bool Window::run_message_loop() {
	MSG msg;

	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		if(msg.message == WM_QUIT) return false;
		DispatchMessage(&msg);
	}

	return true;
}

LRESULT Window::dispatch_message(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam) {
	Window *self = reinterpret_cast<Window *>(GetWindowLong(hWnd,GWL_USERDATA));

	if(!self) {
		char class_name[sizeof(Window *)*2+1];
		GetClassName(hWnd,class_name,sizeof(class_name)/sizeof(class_name[0]));
		sscanf(class_name,"%p",&self);
		SetWindowLong(hWnd,GWL_USERDATA,reinterpret_cast<LONG>(self));
		self->handle_ = hWnd;
	}

	return self->process_message(uMsg,wParam,lParam);
}

LRESULT Window::process_message(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(handle_, uMsg, wParam, lParam);
}

}
