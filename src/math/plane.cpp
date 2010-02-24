//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <assert.h>
#include <luabind/luabind.hpp>
#include "aabb.h"
#include "obb.h"
#include "plane.h"

namespace math
{

template<class T> typename plane<T>::vec_t
plane<T>::get_origin() const
{
	// Ax + By + Cz + D = 0 => A(x - D/A) + By + Cz = 0 etc.

	if (abs(A) > EPSILON) return vec_t(D / A, 0, 0);
	if (abs(B) > EPSILON) return vec_t(0, D / B, 0);
	if (abs(C) > EPSILON) return vec_t(0, 0, D / C);

	// does not define plane
	assert(0);
	return vec_t(0, 0, 0);
}

template<class T> bool
plane<T>::query_intersection(const plane &p, ray_t &r) const {
	r.a = vec_t(A,B,C) ^ vec_t(p.A, p.B, p.C);
	vec_t dir2 = r.a * r.a;

	scalar_t invdet;
	if(dir2.z > dir2.y && dir2.z > dir2.x && dir2.z > EPSILON)
	{
		/* then get a point on the XY plane */
		invdet = scalar_t(1) / r.a.z;
		/* solve < pl1.x * xpt.x + pl1.y * xpt.y = - pl1.w >
		< pl2.x * xpt.x + pl2.y * xpt.y = - pl2.w > */
		r.r0.x = B * p.D - p.B *D;
		r.r0.y = p.A * D - A * p.D;
		r.r0.z = 0;
	}
	else if (dir2.y > dir2.x && dir2.y > EPSILON)
	{
		/* then get a point on the XZ plane */
		invdet = scalar_t(1) / r.a.y;
		/* solve < pl1.x * xpt.x + pl1.z * xpt.z = -pl1.w >
		< pl2.x * xpt.x + pl2.z * xpt.z = -pl2.w > */
		r.r0.x = C * p.D - p.C * D;
		r.r0.y = 0;
		r.r0.z = p.A * D -A * p.D;
	}
	else if (dir2.x > EPSILON)
	{
		/* then get a point on the YZ plane */
		invdet = scalar_t(1) / r.a.x;
		/* solve < pl1.y * xpt.y + pl1.z * xpt.z = - pl1.w >
		< pl2.y * xpt.y + pl2.z * xpt.z = - pl2.w > */
		r.r0.x = 0;
		r.r0.y = C * p.D - p.C * D;
		r.r0.z = p.B * D -B * p.D;
	}
	else return false;

	r.r0 *= invdet;

	invdet = scalar_t(1) / sqrt(dir2.x + dir2.y + dir2.z);
	r.a *= invdet;

	return true;
}

template<class T> typename plane<T>::classification_t
plane<T>::classify(vec_t const &point) const
{
	scalar_t value = apply(point);
	if (abs(value) < EPSILON) return INTERSECTS;
	return value > 0 ? POSITIVE : NEGATIVE;
}

template<class T> typename plane<T>::classification_t
plane<T>::classify(aabb<ARITY, scalar_t> const &bounds) const
{
	// http://zach.in.tu-clausthal.de/teaching/cg_literatur/lighthouse3d_view_frustum_culling/index.html

	vec_t p_point(A >= 0 ? bounds.hi.x : bounds.lo.x, B >= 0 ? bounds.hi.y : bounds.lo.y, C >= 0 ? bounds.hi.z : bounds.lo.z);
	vec_t n_point(A < 0 ? bounds.hi.x : bounds.lo.x, B < 0 ? bounds.hi.y : bounds.lo.y, C < 0 ? bounds.hi.z : bounds.lo.z);

	classification_t pc = classify(p_point), nc = classify(n_point);

	return pc == nc ? pc : INTERSECTS;
}

template<class T> typename plane<T>::classification_t
plane<T>::classify(obb<ARITY, scalar_t> const &bounds) const
{
	plane p;
	bounds.forward_plane(p, *this);
	return p.classify(aabb<ARITY, scalar_t>(vec_t(0, 0, 0), vec_t(1, 1, 1)));
}

template plane<>;

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
