
#include <luabind/luabind.hpp>
#include "static_model.h"

void graphic::StaticModel::bind(lua_State *L)
{
    using namespace luabind;

    module_(L, "graphic")
    [
        class_<StaticModel, Model>("StaticModel")
        .def(constructor<Renderer *>())
    ];
}
