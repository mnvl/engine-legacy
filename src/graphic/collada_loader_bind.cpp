
#include <luabind/luabind.hpp>
#include "collada_loader.h"

void graphic::ColladaLoader::bind(lua_State *L)
{
    using namespace luabind;

    module_(L, "graphic")
    [
        class_<ColladaLoader>("ColladaLoader")
        .def(constructor<std::string const &, Model *, Renderer *>())
        .def("load", &ColladaLoader::load)
    ];
}

