//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2011 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <exception/winapi_error.h>
#include <platform/window.h>

namespace platform
{

Window::Window(char const *title):
	handle_(0)
{
	static int seed = 1;

	char class_name[256];
	sprintf(class_name,"%p %d", this, ++seed);

	WNDCLASSEX wc;
	wc.cbSize = sizeof(wc);
	wc.style = 0;
	wc.lpfnWndProc = dispatch_message;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(0);
	wc.hIcon = 0;
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = 0;
	wc.lpszMenuName = 0;
	wc.lpszClassName = class_name;
	wc.hIconSm = 0;
	if(!RegisterClassEx(&wc)) throw exception::winapi_error();

	handle_ = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, wc.lpszClassName, title, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		0, 0, wc.hInstance, 0);
	if(!handle_) throw exception::winapi_error();
}

Window::Window(luabind::object const &parent_handle):
	handle_(0)
{
	static int seed = 1;

	char class_name[256];
	sprintf(class_name,"%p %d", this, ++seed);

	WNDCLASSEX wc;
	wc.cbSize = sizeof(wc);
	wc.style = 0;
	wc.lpfnWndProc = dispatch_message;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(0);
	wc.hIcon = 0;
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = 0;
	wc.lpszMenuName = 0;
	wc.lpszClassName = class_name;
	wc.hIconSm = 0;
	if(!RegisterClassEx(&wc)) throw exception::winapi_error();

	handle_ = CreateWindowEx(0, wc.lpszClassName, 0, WS_CHILD | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		(HWND) lua_touserdata(parent_handle.interpreter(), -1), 0, wc.hInstance, 0);
	if(!handle_) throw exception::winapi_error();
}

Window::~Window() {
	if (handle_) {
		DestroyWindow(handle_);
		handle_ = 0;
	}
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
	case WM_MOUSEMOVE:
	{
		POINT p;
		p.x = LOWORD(lParam);
		p.y = HIWORD(lParam);
		ScreenToClient(GetParent(handle_), &p);
		mouse_position_ = math::vec<2>(float(p.x), float(p.y));

		// note: fall through
	}

	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
		mouse_buttons_.A = ((wParam & MK_LBUTTON) != 0);
		mouse_buttons_.B = ((wParam & MK_MBUTTON) != 0);
		mouse_buttons_.C = ((wParam & MK_RBUTTON) != 0);
		break;

	case WM_SIZE:
		signal_resize();
		break;

	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(handle_, uMsg, wParam, lParam);
}

}
