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
#include "ray.h"

namespace math
{

namespace
{

template<class T> T distance(ray<2,T> const &r1, ray<2,T> const &r2)
{
	if (!r1.a.is_collinear(r2.a)) return 0;
	return (r1.r0 - r2.r0).length();
}

template<class T> T distance(ray<3, T> const &r1, ray<3, T> const &r2)
{
	// http://softsurfer.com/Archive/algorithm_0106/algorithm_0106.htm
	vec<3,T> w0 = r1.r0 - r2.r0;
	T a = r1.a & r1.a;
	T b = r1.a & r2.a;
	T c = r2.a & r2.a;
	T d = r1.a & w0;
	T e = r2.a & w0;

	T denom = a * c - square(b); // non-negative
	if (denom < EPSILON) return 0;

	T s = (b * e - c * d) / denom;
	T t = (a * e - b * d) / denom;

	return (r1.apply(s) - r2.apply(t)).length();
}

}

template<int N, class T> T
ray<N, T>::distance(ray const &r) const
{
	return math::distance(*this, r);
}

template class ray<2>;
template class ray<3>;

void bind_ray(lua_State *L)
{
	using namespace luabind;

	module(L, "math")
	[
		class_<ray<2> >("ray2")
		.def(constructor<>())
		.def(constructor<const vec<2> &, const vec<2> &>())
		.def("set", &ray<2>::set)
		.def("apply", &ray<2>::apply)
		.def_readwrite("origin", &ray<2>::r0)
		.def_readwrite("direction", &ray<2>::a),

		class_<ray<3> >("ray3")
		.def(constructor<>())
		.def(constructor<const vec<3> &, const vec<3> &>())
		.def("set", &ray<3>::set)
		.def("apply", &ray<3>::apply)
		.def_readwrite("r0", &ray<3>::r0)
		.def_readwrite("a", &ray<3>::a)
	];
}

}
