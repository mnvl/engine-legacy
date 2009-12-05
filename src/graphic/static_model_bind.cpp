
#include <luabind/luabind.hpp>
#include "static_model.h"

void graphic::StaticModel::bind(lua_State *L)
{
    using namespace luabind;

    module_(L, "graphic")
    [
        class_<StaticModel, Model, boost::shared_ptr<StaticModel> >("StaticModel")
		.def(constructor<StaticModel const &>())
        .def(constructor<boost::shared_ptr<Renderer> const &>())
    ];
}
