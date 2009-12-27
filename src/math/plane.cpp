
#include <luabind/luabind.hpp>
#include "plane.h"

namespace math
{

void bind_plane(lua_State *L)
{
	using namespace luabind;

	module(L, "math")
	[
		class_<plane<> >("plane")
		.def(constructor<>())
		.def(constructor<const vec<3> &,const vec<3> &>())
		.def("trace", (scalar (plane<>::*)(const ray<3> &) const) &plane<>::trace)
	];
}

}
