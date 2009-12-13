#pragma once

#include <dinput.h>
#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <lua.h>
#include <util/boost_com_intrusive_ptr.h>
#include <platform/window.h>

namespace platform
{

class Input {
public:
	Input(boost::shared_ptr<Window> const &window_ptr);
    ~Input();

	boost::shared_ptr<Window> const &get_window() const
    {
        return window_;
    }

	IDirectInput8 *get_driver() const
    {
        return driver_.get();
    }

	static void bind(lua_State *L);

private:
	boost::shared_ptr<Window> const window_;
    boost::intrusive_ptr<IDirectInput8> driver_;
};

}
