
#include <boost/shared_ptr.hpp>
#include <luabind/luabind.hpp>
#include "window.h"

void platform::Window::bind(lua_State *L)
{
	using namespace luabind;

    module_(L, "platform")
    [
        class_<Window, boost::shared_ptr<Window> >("Window")
		.def(constructor<char const *>())
		.def("run_message_loop", &Window::run_message_loop)
		.def("get_mouse_position", &Window::get_mouse_position)
		.def("get_mouse_buttons", &Window::get_mouse_buttons)
    ];
}
