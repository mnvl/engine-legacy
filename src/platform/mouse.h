#pragma once
//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <dinput.h>
#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <math/vec.h>
#include <util/boost_com_intrusive_ptr.h>

namespace platform
{

class Input;

class Mouse {
public:
	Mouse(boost::shared_ptr<Input> const &input_ptr, bool exclusive = false, bool absolute = false);
    ~Mouse();

	void update();

    math::vec<3> get_position() const
    {
        return math::vec<3>((math::scalar) state_.lX, (math::scalar) state_.lY, (math::scalar) state_.lZ);
    }

	static void bind(lua_State *L);

private:
	static const int SAMPLE_BUFFER_SIZE = 256;

	boost::shared_ptr<Input> const &input_;
	boost::intrusive_ptr<IDirectInputDevice8> device_;

	DIMOUSESTATE2 state_;
};

}
