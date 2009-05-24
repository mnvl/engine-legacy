#pragma once

#include <dinput.h>
#include <boost/intrusive_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <util/boost_com_intrusive_ptr.h>
#include <platform/window.h>
#include <platform/keyboard.h>
#include <platform/mouse.h>

namespace platform
{

class input {
public:
	input(window *window_ptr);
    ~input();

    window *get_window() const
    {
        return window_;
    }

	IDirectInput8 *get_driver() const
    {
        return driver_.get();
    }

    keyboard *get_keyboard() const
    {
        return keyboard_.get();
    }

    mouse *get_mouse() const
    {
        return mouse_.get();
    }

    void update();

private:
    window *window_;
    boost::intrusive_ptr<IDirectInput8> driver_;

    boost::scoped_ptr<keyboard> keyboard_;
    boost::scoped_ptr<mouse> mouse_;

	void create();
};

}
