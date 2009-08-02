
#include <exception/directx_error.h>
#include <platform/input.h>

namespace platform
{

Input::Input(Window *window_ptr):
	window_(window_ptr)
{
    create();
}

Input::~Input() {
}

void Input::create() {
	HRESULT hr;

	IDirectInput8 *di;
	hr = DirectInput8Create(GetModuleHandle(0),DIRECTINPUT_VERSION,
		IID_IDirectInput8,(void**) &di,0);
	if(FAILED(hr)) throw exception::directx_error(hr);
	driver_.reset(di);
    di->Release();

	keyboard_.reset(new Keyboard(this));
	mouse_.reset(new Mouse(this));
}

void Input::update() {
    keyboard_->update();
    mouse_->update();
}

}
