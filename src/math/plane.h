//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#pragma once

#include <lua.h>
#include "scalar.h"
#include "vec.h"
#include "ray.h"

namespace math {

template<class T = scalar>
class plane {
public:
	typedef T scalar_type;
	typedef std::numeric_limits<scalar_type> limits_type;
	typedef size_t size_type;
	static const size_type arity=3;
	typedef vec<arity,scalar_type> vec_type;
	typedef ray<arity,scalar_type> ray_type;
	typedef plane<scalar_type> plane_type;

	// данные
	scalar_type A, B, C, D;

	// конструкторы
	plane() {
	}

	plane(const vec_type &o,const vec_type &n) {
		construct(o,n);
	}

	void construct(const vec_type &o,const vec_type &n) {
		A = n.x;
		B = n.y;
		C = n.z;
		D = -(o & n);
	}

	// расположение точки
	scalar_type position(const vec_type &v) const {
		return A*v.x + B*v.y + C*v.z + D;
	}

	// пересечение с прямой
	bool intersects(const ray_type &r) const {
		return abs(A*r.a.x + B*r.a.y + C*r.a.z)>EPSILON
			|| contains(r);
	}

	bool contains(vec_type const &p) const {
		return abs(A * p.x + B * p.y + C * p.z + D) < EPSILON;
	}

	bool contains(const ray_type &r) const {
		return abs(A*r.a.x + B*r.a.y + C*r.a.z) < EPSILON && contains(r.r0);
	}

	bool parallel(const ray_type &r) const {
		return !intersects(r);
	}

	scalar_type trace(const ray_type &r) const {
		scalar_type a_n = A*r.a.x + B*r.a.y + C*r.a.z;
		return -(A*r.r0.x + B*r.r0.y + C*r.r0.z + D) / a_n;
	}

	// найти пересечение с лучом r
	// r -- данный луч
	// t -- параметр в уравнении прямой r = r.origin + r.direction * t;
	bool trace(const ray_type &r,scalar_type &t,scalar_type t_min = 0, scalar_type t_max = 1) const {
		t = trace(r);
		return t >= t_min && t <= t_max;
	}

	// найти пересечение с лучом r
	// r -- данный луч
	// t -- параметр в уравнении прямой r = r.origin + r.direction * t;
	// v -- точка пересечения
	bool trace(const ray_type &r,scalar_type &t,vec_type &v,scalar_type t_min = 0, scalar_type t_max = 1) const {
		if(!trace(r,t,t_min,t_max)) return false;
		v = r.r0 + r.a * t;
		return true;
	}

	// найти пересечение с плоскостью pl
	// pl -- плоскость
	// result -- искомая прямая пересечения
	// если плоскости параллельны -- возвращает false
	bool intersect(const plane_type &pl, ray_type &r) const {
		r.a = vec_type(A,B,C) ^ vec_type(pl.A, pl.B, pl.C);
		vec_type dir2 = r.a * r.a;

		scalar_type invdet;
		if(dir2.z > dir2.y && dir2.z > dir2.x && dir2.z > EPSILON)
		{
			/* then get a point on the XY plane */
			invdet = scalar_type(1) / r.a.z;
			/* solve < pl1.x * xpt.x + pl1.y * xpt.y = - pl1.w >
			< pl2.x * xpt.x + pl2.y * xpt.y = - pl2.w > */
			r.r0.x = B * pl.D - pl.B *D;
			r.r0.y = pl.A * D - A * pl.D;
			r.r0.z = 0;
		}
		else if (dir2.y > dir2.x && dir2.y > EPSILON)
		{
			/* then get a point on the XZ plane */
			invdet = scalar_type(1) / r.a.y;
			/* solve < pl1.x * xpt.x + pl1.z * xpt.z = -pl1.w >
			< pl2.x * xpt.x + pl2.z * xpt.z = -pl2.w > */
			r.r0.x = C * pl.D - pl.C * D;
			r.r0.y = 0;
			r.r0.z = pl.A * D -A * pl.D;
		}
		else if (dir2.x > EPSILON)
		{
			/* then get a point on the YZ plane */
			invdet = scalar_type(1) / r.a.x;
			/* solve < pl1.y * xpt.y + pl1.z * xpt.z = - pl1.w >
			< pl2.y * xpt.y + pl2.z * xpt.z = - pl2.w > */
			r.r0.x = 0;
			r.r0.y = C * pl.D - pl.C * D;
			r.r0.z = pl.B * D -B * pl.D;
		}
		else return false;

		r.r0 *= invdet;

		invdet = scalar_type(1) / sqrt(dir2.x + dir2.y + dir2.z);
		r.a *= invdet;

		return true;
	}
};

void bind_plane(lua_State *L);

}
