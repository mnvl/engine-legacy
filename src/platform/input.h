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

class Input {
public:
	Input(Window *window_ptr);
    ~Input();

    Window *get_window() const
    {
        return window_;
    }

	IDirectInput8 *get_driver() const
    {
        return driver_.get();
    }

    Keyboard *get_keyboard() const
    {
        return keyboard_.get();
    }

    Mouse *get_mouse() const
    {
        return mouse_.get();
    }

    void update();

private:
    Window *window_;
    boost::intrusive_ptr<IDirectInput8> driver_;

    boost::scoped_ptr<Keyboard> keyboard_;
    boost::scoped_ptr<Mouse> mouse_;

	void create();
};

}
