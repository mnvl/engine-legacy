
#include <luabind/lua_include.hpp>
#include <luabind/luabind.hpp>
#include <platform/window.h>
#include <platform/keyboard.h>
#include <platform/mouse.h>
#include <platform/input.h>


namespace bootstrap
{

void bind_platform(lua_State *L)
{
    using namespace luabind;
    using namespace platform;

    module(L, "platform")
    [
        class_<window>("window")
        .def(constructor<char const *>())
        .def("run_message_loop", &window::run_message_loop),

        class_<keyboard>("keyboard")
        .enum_("key_t")
        [
            value("KEY_W", keyboard::KEY_W),
            value("KEY_S", keyboard::KEY_S),
            value("KEY_A", keyboard::KEY_A),
            value("KEY_D", keyboard::KEY_D)
        ]
        .def("is_key_pressed", &keyboard::is_key_pressed),

        class_<mouse>("mouse"),

        class_<input>("input")
        .def(constructor<window *>())
        .def("get_keyboard", &input::get_keyboard)
        .def("get_mouse", &input::get_mouse)
        .def("update", &input::update)
    ];
}

}
