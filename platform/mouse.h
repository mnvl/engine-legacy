#pragma once

#include <dinput.h>
#include <boost/intrusive_ptr.hpp>
#include <math/vec.h>
#include <util/boost_com_intrusive_ptr.h>

namespace platform
{

class input;

class mouse {
public:
	mouse(input *input_ptr);
    ~mouse();

	void update();

    math::vec<3> get_delta() const
    {
        return math::vec<3>((math::scalar) state_.lX, (math::scalar) state_.lY, (math::scalar) state_.lZ);
    }

private:
	static const int SAMPLE_BUFFER_SIZE = 256;

	input *input_;
	boost::intrusive_ptr<IDirectInputDevice8> device_;

	DIMOUSESTATE2 state_;

	void create();
};

}
