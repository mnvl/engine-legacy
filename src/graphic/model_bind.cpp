
#include <boost/shared_ptr.hpp>
#include <luabind/luabind.hpp>
#include "model.h"

void graphic::Model::bind(lua_State *L)
{
    using namespace luabind;

    module_(L, "graphic")
    [
        class_<Model, boost::shared_ptr<Model> >("Model")
		.def("load_from_collada", &Model::load_from_collada)
		.def("set_animation_time", &Model::set_animation_time)
		.def("draw", &Model::draw)
    ];
}

