//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>
#include "vec.h"

namespace math
{

void bind_vec(lua_State *L)
{
	using namespace luabind;

	module(L, "math")
	[
		class_<vec<2> >("vec2")
		.def(constructor<>())
		.def(constructor<scalar, scalar>())
		.def_readwrite("x", &vec<2>::x)
		.def_readwrite("y", &vec<2>::y)
		.def("set", &vec<2>::set)
		.def(const_self < const_self)
		.def(tostring(const_self)),

		class_<vec<3> >("vec3")
		.def(constructor<>())
		.def(constructor<scalar, scalar, scalar>())
		.def(constructor<vec<3> const &>())
		.def_readwrite("x", &vec<3>::x)
		.def_readwrite("y", &vec<3>::y)
		.def_readwrite("z", &vec<3>::z)
		.def(-const_self)
		.def(const_self + const_self)
		.def(const_self - const_self)
		.def(const_self * other<scalar>())
		.def(const_self / other<scalar>())
		.def(const_self < const_self)
		.def(const_self == const_self)
		.def("length", &vec<3>::length)
		.def("length_sq", &vec<3>::length_sq)
		.def("normalize", &vec<3>::normalize)
		.def("set", &vec<3>::set)
		.def("perpendicular_xy", &vec<3>::perpendicular_xy)
		.def("perpendicular_yz", &vec<3>::perpendicular_yz)
		.def("perpendicular_xz", &vec<3>::perpendicular_xz)
		.def(tostring(const_self)),

		class_<vec<3,bool> >("vec3b")
		.def(constructor<>())
		.def(constructor<bool, bool, bool>())
		.def(constructor<vec<3, bool> const &>())
		.def_readwrite("x", &vec<3,bool>::x)
		.def_readwrite("y", &vec<3,bool>::y)
		.def_readwrite("z", &vec<3,bool>::z)
		.def(const_self < const_self)
		.def(tostring(const_self)),

		class_<vec<4> >("vec4")
		.def(constructor<>())
		.def(constructor<scalar, scalar, scalar, scalar>())
		.def(constructor<vec<4> const &>())
		.def_readwrite("x", &vec<4>::x)
		.def_readwrite("y", &vec<4>::y)
		.def_readwrite("z", &vec<4>::z)
		.def_readwrite("w", &vec<4>::w)
		.def(const_self < const_self)
		.def(tostring(const_self)),

		def("dot_product", (scalar (*)(const vec<2> &,const vec<2> &)) dot_product),
		def("dot_product", (scalar (*)(const vec<3> &,const vec<3> &)) dot_product),
		def("dot_product", (scalar (*)(const vec<4> &,const vec<4> &)) dot_product),

		def("cross_product", (vec<3> (*)(const vec<3> &,const vec<3> &)) operator ^),
		def("cross_product", (void (*)(vec<3> &, const vec<3> &,const vec<3> &)) cross_product)
	];
}

}
