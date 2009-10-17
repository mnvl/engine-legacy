
#include <luabind/luabind.hpp>
#include "animated_model.h"

void graphic::AnimatedModel::bind(lua_State *L)
{
    using namespace luabind;

    module_(L, "graphic")
    [
        class_<AnimatedModel, Model>("AnimatedModel")
        .def(constructor<Renderer *>())
    ];
}
