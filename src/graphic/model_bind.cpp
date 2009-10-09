
#include <luabind/luabind.hpp>
#include "model.h"

void graphic::Model::bind(lua_State *L)
{
    using namespace luabind;

    module_(L, "graphic")
    [
        class_<Model>("Model")
		.def("draw", &Model::draw)
    ];
}

