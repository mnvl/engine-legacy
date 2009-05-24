#pragma once

#include <dinput.h>
#include <boost/intrusive_ptr.hpp>
#include <util/boost_com_intrusive_ptr.h>

namespace platform
{

class input;

class mouse {
public:
    enum {
        SOURCE_AXIS_X		= 0,
        SOURCE_AXIS_Y		= 1,
        SOURCE_AXIS_Z		= 2,
        SOURCE_BUTTON_1		= 3,
        SOURCE_BUTTON_2		= 4,
        SOURCE_BUTTON_3		= 5,
        SOURCE_BUTTON_4		= 6,
        SOURCE_BUTTON_5		= 7,
        SOURCE_BUTTON_7		= 8,
        SOURCE_BUTTON_8		= 9
    };

	mouse(input *input_ptr);
    ~mouse();

	void update();

private:
	static const int SAMPLE_BUFFER_SIZE = 256;

	input *input_;
	boost::intrusive_ptr<IDirectInputDevice8> device_;

	DIMOUSESTATE2 state_;

	void create();
};

}
