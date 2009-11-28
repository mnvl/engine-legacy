
#include <luabind/luabind.hpp>
#include "camera.h"

void graphic::Camera::bind(lua_State *L)
{
	using namespace luabind;

	module(L, "graphic")
	[
		class_<Camera>("Camera")
		.def(constructor<>())
		.def("set_perspective_projection", &Camera::set_perspective_projection)
		.def("set_orthographic_projection", &Camera::set_orthographic_projection)
		.def("set_eye_lookat_up", &Camera::set_eye_lookat_up)
		.def("get_eye_point", &Camera::get_eye_point)
	];
}
