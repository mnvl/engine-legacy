
#include <exception/directx_error.h>
#include <platform/input.h>

namespace platform
{

Input::Input(boost::shared_ptr<Window> const &window_ptr):
	window_(window_ptr)
{
	HRESULT hr;

	IDirectInput8 *di;
	hr = DirectInput8Create(GetModuleHandle(0),DIRECTINPUT_VERSION,
		IID_IDirectInput8,(void**) &di,0);
	if(FAILED(hr)) throw exception::directx_error(hr);
	driver_.reset(di);
    di->Release();
}

Input::~Input() {
}

}
