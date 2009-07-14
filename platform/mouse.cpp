#if defined(WIN32)

#include <exception/directx_error.h>
#include <platform/input.h>
#include <platform/mouse.h>

namespace platform
{

mouse::mouse(input *input_ptr):
	input_(input_ptr)
{
	create();
}

mouse::~mouse() {
	device_->Unacquire();
}

void mouse::create() {
	HRESULT hr;

    // Obtain an interface to the system mouse device.
	IDirectInputDevice8 *id;
    if (FAILED(hr = input_->get_driver()->CreateDevice(GUID_SysMouse, &id, NULL ) ) )
         throw exception::directx_error(hr);
	device_.reset(id);
    id->Release();

    // Set the data format to "mouse format" - a predefined data format
    //
    // A data format specifies which controls on a device we
    // are interested in, and how they should be reported.
    //
    // This tells DirectInput that we will be passing a
    // DIMOUSESTATE2 structure to IDirectInputDevice::GetDeviceState.
    if (FAILED(hr = device_->SetDataFormat(&c_dfDIMouse2 ) ) )
        throw exception::directx_error(hr);

    // Set the cooperativity level to let DirectInput know how
    // this device should interact with the system and with other
    // DirectInput applications.
	hr = device_->SetCooperativeLevel(input_->get_window()->get_handle(), DISCL_EXCLUSIVE|DISCL_FOREGROUND );
	if (FAILED(hr)) throw exception::directx_error(hr);

    // IMPORTANT STEP TO USE BUFFERED DEVICE DATA!
    //
    // DirectInput uses unbuffered I/O (buffer size = 0) by default.
    // If you want to read buffered data, you need to set a nonzero
    // buffer size.
    //
    // Set the buffer size to SAMPLE_BUFFER_SIZE (defined above) elements.
    //
    // The buffer size is a DWORD property associated with the device.
    DIPROPDWORD dipdw;
    dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj        = 0;
    dipdw.diph.dwHow        = DIPH_DEVICE;
    dipdw.dwData            = SAMPLE_BUFFER_SIZE; // Arbitary buffer size

    hr = device_->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
    if (FAILED(hr)) throw exception::directx_error(hr);

    dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj        = 0;
    dipdw.diph.dwHow        = DIPH_DEVICE;
    dipdw.dwData            = DIPROPAXISMODE_REL; //or ABS here
    hr = device_->SetProperty(DIPROP_AXISMODE, &dipdw.diph);
    if (FAILED(hr)) throw exception::directx_error(hr);

    // Acquire the newly created device
    device_->Acquire();
}

void mouse::update() {
    HRESULT       hr;
    DIMOUSESTATE2 dims2;      // DirectInput mouse state structure

    // Get the input's device state, and put the state in dims
    memset(&dims2,0,sizeof(dims2));
    hr = device_->GetDeviceState(sizeof(DIMOUSESTATE2), &dims2);
    if (FAILED(hr))
    {
        // DirectInput may be telling us that the input stream has been
        // interrupted.  We aren't tracking any state between polls, so
        // we don't have any special reset that needs to be done.
        // We just re-acquire and try again.

        // If input is lost then acquire and keep trying
        hr = device_->Acquire();
        while(hr == DIERR_INPUTLOST)
            hr = device_->Acquire();

        // Update the dialog text
        if (hr == DIERR_NOTACQUIRED)
            throw exception::directx_error(hr);

        // hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
        // may occur when the app is minimized or in the process of
        // switching, so just try again later
        return;
    }

    state_ = dims2;
}

}

#endif
