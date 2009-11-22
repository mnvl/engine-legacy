
#include <boost/shared_ptr.hpp>
#include <luabind/luabind.hpp>
#include <platform/window.h>
#include "renderer.h"

void graphic::Renderer::bind(lua_State *L)
{
    using namespace luabind;

    module_(L, "graphic")
    [
        class_<Renderer, boost::shared_ptr<Renderer> >("Renderer")
		.def(constructor<boost::shared_ptr<platform::Window> const &>())
		.def("begin_frame", &Renderer::begin_frame)
		.def("finish_frame", &Renderer::finish_frame)
		.def("set_view_projection_from_camera", &Renderer::set_view_projection_from_camera)
        .def("set_world_matrix", &Renderer::set_world_matrix)
    ];
}

