
#include <exception/directx_error.h>
#include <platform/input.h>
#include <platform/mouse.h>

namespace platform
{

Mouse::Mouse(boost::shared_ptr<Input> const &input_ptr, bool exclusive, bool absolute):
	input_(input_ptr)
{
	HRESULT hr;

	IDirectInputDevice8 *id;
    if (FAILED(hr = input_->get_driver()->CreateDevice(GUID_SysMouse, &id, 0)))
         throw exception::directx_error(hr);
	device_.reset(id);
    id->Release();

    if (FAILED(hr = device_->SetDataFormat(&c_dfDIMouse2)))
        throw exception::directx_error(hr);

	hr = device_->SetCooperativeLevel(input_->get_window()->get_handle(),
		DISCL_FOREGROUND | (exclusive ? DISCL_EXCLUSIVE : DISCL_NONEXCLUSIVE));
	if (FAILED(hr)) throw exception::directx_error(hr);

    DIPROPDWORD dipdw;
    dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj        = 0;
    dipdw.diph.dwHow        = DIPH_DEVICE;
    dipdw.dwData            = SAMPLE_BUFFER_SIZE;

    hr = device_->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
    if (FAILED(hr)) throw exception::directx_error(hr);

    dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj        = 0;
    dipdw.diph.dwHow        = DIPH_DEVICE;
	dipdw.dwData            = absolute ? DIPROPAXISMODE_ABS : DIPROPAXISMODE_REL;
    hr = device_->SetProperty(DIPROP_AXISMODE, &dipdw.diph);
    if (FAILED(hr)) throw exception::directx_error(hr);

    device_->Acquire();
}

Mouse::~Mouse() {
	device_->Unacquire();
}

void Mouse::update() {
    HRESULT       hr;
    DIMOUSESTATE2 dims2;

    memset(&dims2,0,sizeof(dims2));
    hr = device_->GetDeviceState(sizeof(DIMOUSESTATE2), &dims2);

    if (FAILED(hr))
    {
        hr = device_->Acquire();
        while(hr == DIERR_INPUTLOST) hr = device_->Acquire();

        // failure can occur when the app is minimized or in the process of switching,
		// so we just try again later
        if (hr == DIERR_NOTACQUIRED) throw exception::directx_error(hr);

        return;
    }

    state_ = dims2;
}

}
