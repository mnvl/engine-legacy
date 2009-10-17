
#include <luabind/luabind.hpp>
#include "collada_loader.h"

void graphic::ColladaLoader::bind(lua_State *L)
{
    using namespace luabind;

    module_(L, "graphic")
    [
        class_<ColladaLoader>("ColladaLoader")
        .def(constructor<Model *>())
		.def("load", &ColladaLoader::load)
		.def("build_model", &ColladaLoader::build_model)
    ];
}

