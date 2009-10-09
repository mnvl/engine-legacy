
#include <luabind/luabind.hpp>
#include "renderer.h"

void graphic::Renderer::bind(lua_State *L)
{
    using namespace luabind;

    module_(L, "graphic")
    [
        class_<Renderer>("Renderer")
    ];
}

