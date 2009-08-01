
#include <exception/directx_error.h>
#include <platform/input.h>
#include <platform/keyboard.h>

namespace platform
{

keyboard::keyboard(input *input_ptr):
	input_(input_ptr)
{
	memset(state_, 0, sizeof(state_));
	create();
}

keyboard::~keyboard() {
	device_->Unacquire();
}

void keyboard::create() {
	HRESULT hr;

	// Obtain an interface to the system keyboard device.
	IDirectInputDevice8 *id;
	if (FAILED(hr = input_->get_driver()->CreateDevice(GUID_SysKeyboard, &id, NULL)))
		throw exception::directx_error(hr);
	device_.reset(id);
    id->Release();

	// Set the data format to "keyboard format" - a predefined data format 
	//
	// A data format specifies which controls on a device we
	// are interested in, and how they should be reported.
	//
	// This tells DirectInput that we will be passing an array
	// of 256 bytes to IDirectInputDevice::GetDeviceState.
	if (FAILED(hr = device_->SetDataFormat(&c_dfDIKeyboard)))
		throw exception::directx_error(hr);

	// Set the cooperativity level to let DirectInput know how
	// this device should interact with the system and with other
	// DirectInput applications.
	hr = device_->SetCooperativeLevel(input_->get_window()->get_handle(), DISCL_NONEXCLUSIVE |
		DISCL_FOREGROUND);
	if (FAILED(hr)) throw exception::directx_error(hr);

	// IMPORTANT STEP TO USE BUFFERED DEVICE DATA!
	//
	// DirectInput uses unbuffered I/O (buffer size = 0) by default.
	// If you want to read buffered data, you need to set a nonzero
	// buffer size.
	//
	// Set the buffer size to DINPUT_BUFFERSIZE (defined above) elements.
	//
	// The buffer size is a DWORD property associated with the device.
	DIPROPDWORD dipdw;

	dipdw.diph.dwSize	   = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj		= 0;
	dipdw.diph.dwHow		= DIPH_DEVICE;
	dipdw.dwData			= SAMPLE_BUFFER_SIZE; // Arbitary buffer size

	if (FAILED(hr = device_->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph)))
		throw exception::directx_error(hr);

	// Acquire the newly created device
	device_->Acquire();
}

void keyboard::update() {
	HRESULT hr;

	// Get the input's device state, and put the state in dims
	hr = device_->GetDeviceState(sizeof(state_), state_);
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
}

char keyboard::get_character_by_key(key_t key) {
	static char table[SAMPLE_BUFFER_SIZE+1]={
		0,	    //					0
		0,	    // DIK_ESCAPE		1
		'1',	// DIK_1			2
		'2',	//					3
		'3',	//					4
		'4',	//					5
		'5',	//					6
		'6',	//					7
		'7',	//					8
		'8',	//					9
		'9',	//					A
		'0',	//					B
		'-',	// DIK_MINUS		C
		'=',	// DIK_EQUALS		D
		0,	    // DIK_BACK			E
		0,	    // DIK_TAB			F
		'q',	// DIK_Q		   10
		'w',	//				   11
		'e',	//				   12
		'r',	//				   13
		't',	//				   14
		'y',	//				   15
		'u',	//				   16
		'i',	//				   17
		'o',	//				   18
		'p',	//				   19
		'[',	// DIK_LBRACKET	   1A
		']',	// DIK_RBRACKET	   1B
		'\r',	// DIK_RETURN	   1C
		0,		// DIK_LCONTROL	   1D
		'a',	// DIK_A		   1E
		's',	//				   1F
		'd',	//				   20
		'f',	//				   21
		'g',	//				   22
		'h',	//				   23
		'j',	//				   24
		'k',	//				   25
		'l',	//				   26
		';',	// DIK_SEMICOLON   27
		'\'',	// DIK_APOSTROPHE  28
		0,		// DIK_GRAVE	   29
		0,		// DIK_LSHIFT	   2A
		'\\',	// DIK_BACKSLASH   2B
		'z',	// DIK_Z		   2C
		'x',	//				   2D
		'c',	//				   2E
		'v',	//				   2F
		'b',	//				   30
		'n',	//				   31
		'm',	//				   32
		',',	// DIK_COMMA	   33
		'.',	// DIK_PERIOD	   34
		'/',	// DIK_SLASH	   35
		0,		// DIK_RSHIFT	   36
		'*',	// DIK_MULTIPLY	   37
		0,		// DIK_LMENU	   38
		' '		// DIK_SPACE	   39
	};

	return table[key];
}

}
