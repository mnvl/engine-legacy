
#include <luabind/luabind.hpp>
#include <platform/window.h>
#include "renderer.h"

void graphic::Renderer::bind(lua_State *L)
{
    using namespace luabind;

    module_(L, "graphic")
    [
        class_<Renderer>("Renderer")
		.def(constructor<platform::Window *>())
		.def("begin_frame", &Renderer::begin_frame)
		.def("finish_frame", &Renderer::finish_frame)
		.def("set_view_projection_from_camera", &Renderer::set_view_projection_from_camera)
        .def("set_world_matrix", &Renderer::set_world_matrix)
    ];
}

