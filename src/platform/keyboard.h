#pragma once

#include <dinput.h>
#include <boost/intrusive_ptr.hpp>
#include <util/boost_com_intrusive_ptr.h>
#include <platform/window.h>

namespace platform
{

class input;

class keyboard {
public:
	enum key_t {
		KEY_ESCAPE			= DIK_ESCAPE,
		KEY_1				= DIK_1,
		KEY_2				= DIK_2,
		KEY_3				= DIK_3,
		KEY_4				= DIK_4,
		KEY_5				= DIK_5,
		KEY_6				= DIK_6,
		KEY_7				= DIK_7,
		KEY_8				= DIK_8,
		KEY_9				= DIK_9,
		KEY_0				= DIK_0,
		KEY_MINUS			= DIK_MINUS,
		KEY_EQUAL			= DIK_EQUALS,
		KEY_BACK			= DIK_BACK,
		KEY_TAB				= DIK_TAB,
		KEY_Q				= DIK_Q,
		KEY_W				= DIK_W,
		KEY_E				= DIK_E,
		KEY_R				= DIK_R,
		KEY_T				= DIK_T,
		KEY_Y				= DIK_Y,
		KEY_U				= DIK_U,
		KEY_I				= DIK_I,
		KEY_O				= DIK_O,
		KEY_P				= DIK_P,
		KEY_LBRACKET	    = DIK_LBRACKET,
		KEY_RBRACKET	  	= DIK_RBRACKET,
		KEY_RETURN			= DIK_RETURN,
		KEY_LCONTROL	    = DIK_LCONTROL,
		KEY_A				= DIK_A,
		KEY_S				= DIK_S,
		KEY_D				= DIK_D,
		KEY_F				= DIK_F,
		KEY_G				= DIK_G,
		KEY_H				= DIK_H,
		KEY_J				= DIK_J,
		KEY_K				= DIK_K,
		KEY_L				= DIK_L,
		KEY_SEMICOLON	    = DIK_SEMICOLON,
		KEY_APOSTROPHE	   	= DIK_APOSTROPHE,
		KEY_GRAVE			= DIK_GRAVE,
		KEY_LSHIFT			= DIK_LSHIFT,
		KEY_BACKSLASH	    = DIK_BACKSLASH,
		KEY_Z				= DIK_Z,
		KEY_X				= DIK_X,
		KEY_C				= DIK_C,
		KEY_V				= DIK_V,
		KEY_B				= DIK_B,
		KEY_N				= DIK_N,
		KEY_M				= DIK_M,
		KEY_COMMA			= DIK_COMMA,
		KEY_PERIOD			= DIK_PERIOD,
		KEY_SLASH			= DIK_SLASH,
		KEY_RSHIFT			= DIK_RSHIFT,
		KEY_MULTIPLY	    = DIK_MULTIPLY,
		KEY_SPACE			= DIK_SPACE,
		KEY_CAPITAL			= DIK_CAPITAL,
		KEY_F1				= DIK_F1,
		KEY_F2				= DIK_F2,
		KEY_F3				= DIK_F3,
		KEY_F4				= DIK_F4,
		KEY_F5				= DIK_F5,
		KEY_F6				= DIK_F6,
		KEY_F7				= DIK_F7,
		KEY_F8				= DIK_F8,
		KEY_F9				= DIK_F9,
		KEY_F10				= DIK_F10,
		KEY_NUMLOCK			= DIK_NUMLOCK,
		KEY_SCROLL			= DIK_SCROLL,
		KEY_NUMPAD7			= DIK_NUMPAD7,
		KEY_NUMPAD8			= DIK_NUMPAD8,
		KEY_NUMPAD9			= DIK_NUMPAD9,
		KEY_SUBTRACT		= DIK_SUBTRACT,
		KEY_NUMPAD4			= DIK_NUMPAD4,
		KEY_NUMPAD5			= DIK_NUMPAD5,
		KEY_NUMPAD6			= DIK_NUMPAD6,
		KEY_ADD				= DIK_ADD,
		KEY_NUMPAD1			= DIK_NUMPAD1,
		KEY_NUMPAD2			= DIK_NUMPAD2,
		KEY_NUMPAD3			= DIK_NUMPAD3,
		KEY_NUMPAD0			= DIK_NUMPAD0,
		KEY_DECIMAL			= DIK_DECIMAL,
		KEY_RCONTROL	    = DIK_RCONTROL,
		KEY_HOME			= DIK_HOME,
		KEY_UP				= DIK_UP,
		KEY_PRIOR			= DIK_PRIOR,
		KEY_LEFT			= DIK_LEFT,
		KEY_RIGHT			= DIK_RIGHT,
		KEY_END				= DIK_END,
		KEY_DOWN			= DIK_DOWN,
		KEY_NEXT			= DIK_NEXT,
		KEY_INSERT			= DIK_INSERT,
		KEY_DELETE			= DIK_DELETE
	};

	static char get_character_by_key(key_t key);

    keyboard(input *input_ptr);
    ~keyboard();

    void update();

    bool is_key_pressed(key_t key)
    {
        return (state_[key] & 0x80) != 0;
    }

private:
	static size_t const SAMPLE_BUFFER_SIZE=256;

	input *input_;
	boost::intrusive_ptr<IDirectInputDevice8> device_;
	BYTE state_[SAMPLE_BUFFER_SIZE];

	void create();
};

}


