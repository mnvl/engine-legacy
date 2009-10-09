
#include <luabind/luabind.hpp>
#include "main.h"

void Main::bind(lua_State *L)
{
    using namespace luabind;

    module(L, "Main")
    [
        def("get_renderer", Main::get_renderer)
    ];
}
