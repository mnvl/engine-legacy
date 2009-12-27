
#include <luabind/luabind.hpp>
#include "ray.h"

namespace math
{

void bind_ray(lua_State *L)
{
	using namespace luabind;

	module(L, "math")
	[
		class_<ray<2> >("ray2")
		.def(constructor<>())
		.def(constructor<const vec<2> &, const vec<2> &>())
		.def("construct", &ray<2>::construct)
		.def("apply", &ray<2>::apply)
		.def_readwrite("origin", &ray<2>::origin)
		.def_readwrite("direction", &ray<2>::direction),

		class_<ray<3> >("ray3")
		.def(constructor<>())
		.def(constructor<const vec<3> &, const vec<3> &>())
		.def("construct", &ray<3>::construct)
		.def("apply", &ray<3>::apply)
		.def_readwrite("origin", &ray<3>::origin)
		.def_readwrite("direction", &ray<3>::direction),

		class_<ray<4> >("ray4")
		.def(constructor<>())
		.def(constructor<const vec<4> &, const vec<4> &>())
		.def("construct", &ray<4>::construct)
		.def("apply", &ray<4>::apply)
		.def_readwrite("origin", &ray<4>::origin)
		.def_readwrite("direction", &ray<4>::direction)
	];
}

}
