
#include <luabind/lua_include.hpp>
#include <luabind/luabind.hpp>
#include <platform/window.h>

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
        .def("run_message_loop", &window::run_message_loop)
    ];
}

}
