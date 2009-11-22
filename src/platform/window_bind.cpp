
#include <boost/shared_ptr.hpp>
#include <luabind/luabind.hpp>
#include "window.h"

void platform::Window::bind(lua_State *L)
{
	using namespace luabind;

    module_(L, "platform")
    [
        class_<Window, boost::shared_ptr<Window> >("Window")
		.def(constructor<>())
		.def("create", &Window::create)
		.def("run_message_loop", &Window::run_message_loop)
    ];
}
