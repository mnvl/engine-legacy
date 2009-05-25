
#include <luabind/lua_include.hpp>
#include <luabind/luabind.hpp>
#include <platform/window.h>
#include <graphic/camera.h>
#include <graphic/static_model.h>
#include <graphic/renderer.h>

namespace bootstrap
{

void bind_graphic(lua_State *L)
{
    using namespace luabind;
    using namespace graphic;

    module(L, "graphic")
    [
        class_<camera>("camera")
        .def(constructor<>())
        .def("set_perspective_projection", &camera::set_perspective_projection)
        .def("set_eye_lookat_up", &camera::set_eye_lookat_up)
        .def("set_position_rotation", &camera::set_position_rotation),

        class_<model>("model")
        .def("load_from_collada", &model::load_from_collada)
        .def("set_animation_time", &model::set_animation_time)
        .def("draw", &model::draw),

        class_<static_model, model>("static_model")
        .def(constructor<renderer *>()),

        class_<renderer>("renderer")
        .def(constructor<platform::window *>())
        .def("begin_frame", &renderer::begin_frame)
        .def("finish_frame", &renderer::finish_frame)
        .def("set_view_projection_from_camera", &renderer::set_view_projection_from_camera)
    ];
}

}
