
#include <boost/shared_ptr.hpp>
#include <luabind/luabind.hpp>
#include "animated_model.h"

void graphic::AnimatedModel::bind(lua_State *L)
{
    using namespace luabind;

    module_(L, "graphic")
    [
        class_<AnimatedModel, Model, boost::shared_ptr<AnimatedModel> >("AnimatedModel")
        .def(constructor<boost::shared_ptr<Renderer> const &>())
	];
}
