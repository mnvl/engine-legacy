
#include <stdio.h>
#include <engine/exception/winapi_error.h>
#include <engine/system/window.h>

namespace engine {
namespace system {

window::window(char const *title):
	handle_(0)
{
	if (title) create(title);
}

window::~window() {
	if (handle_) {
		DestroyWindow(handle_);
		handle_ = 0;
	}
}

void window::create(char const *title) {
	// получить название класса
	char class_name[sizeof(window *)*2+1];
	sprintf(class_name,"%p",this);

	// зарегестрировать класс окна
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
	
	// создать окно
	CreateWindowEx(0, wc.lpszClassName, title, WS_OVERLAPPED|WS_VISIBLE|WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		0, 0, wc.hInstance, 0);
	if(!handle_) throw exception::winapi_error();
}

void window::run_message_loop() {
	MSG msg;
	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
		DispatchMessage(&msg);
	}
}

LRESULT window::dispatch_message(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam) {
	// получить указатель на объект
	window *self = reinterpret_cast<window *>(GetWindowLong(hWnd,GWL_USERDATA));

	// если он еще не записан
	if(!self) {
		char class_name[sizeof(window *)*2+1];
		GetClassName(hWnd,class_name,sizeof(class_name)/sizeof(class_name[0]));
		sscanf(class_name,"%p",&self);
		SetWindowLong(hWnd,GWL_USERDATA,reinterpret_cast<LONG>(self));
		self->handle_ = hWnd;
	}

	return self->process_message(uMsg,wParam,lParam);
}

LRESULT window::process_message(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	return DefWindowProc(handle_, uMsg, wParam, lParam);
}

}
}