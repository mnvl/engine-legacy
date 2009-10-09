
#include "window.h"
#include <luabind/luabind.hpp>

void platform::Window::bind(lua_State *L)
{
	using namespace luabind;

    module_(L, "platform")
    [
        class_<Window>("Window")
		.def(constructor<>())
		.def("create", &Window::create)
		.def("run_message_loop", &Window::run_message_loop)
    ];
}
