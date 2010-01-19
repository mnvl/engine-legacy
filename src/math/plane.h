#pragma once
//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <luabind/lua_include.hpp>
#include "scalar.h"
#include "vec.h"
#include "ray.h"

namespace math {

template<class T = scalar>
class plane {
public:
	typedef T scalar_t;
	typedef std::numeric_limits<scalar_t> limits_t;
	typedef size_t size_t;
	static const size_t ARITY=3;
	typedef vec<ARITY,scalar_t> vec_t;
	typedef ray<ARITY,scalar_t> ray_t;
	typedef plane<scalar_t> plane_t;

	// данные
	scalar_t A, B, C, D;

	// конструкторы
	plane() {
	}

	plane(const vec_t &o,const vec_t &n) {
		construct(o,n);
	}

	void construct(const vec_t &o,const vec_t &n) {
		A = n.x;
		B = n.y;
		C = n.z;
		D = -(o & n);
	}

	// расположение точки
	scalar_t position(const vec_t &v) const {
		return A*v.x + B*v.y + C*v.z + D;
	}

	// пересечение с прямой
	bool intersects(const ray_t &r) const {
		return abs(A*r.a.x + B*r.a.y + C*r.a.z)>EPSILON
			|| contains(r);
	}

	bool contains(vec_t const &p) const {
		return abs(A * p.x + B * p.y + C * p.z + D) < EPSILON;
	}

	bool contains(const ray_t &r) const {
		return abs(A*r.a.x + B*r.a.y + C*r.a.z) < EPSILON && contains(r.r0);
	}

	bool parallel(const ray_t &r) const {
		return !intersects(r);
	}

	scalar_t trace(const ray_t &r) const {
		scalar_t a_n = A*r.a.x + B*r.a.y + C*r.a.z;
		return -(A*r.r0.x + B*r.r0.y + C*r.r0.z + D) / a_n;
	}

	// найти пересечение с лучом r
	// r -- данный луч
	// t -- параметр в уравнении прямой r = r.origin + r.direction * t;
	bool trace(const ray_t &r,scalar_t &t,scalar_t t_min = 0, scalar_t t_max = 1) const {
		t = trace(r);
		return t >= t_min && t <= t_max;
	}

	// найти пересечение с лучом r
	// r -- данный луч
	// t -- параметр в уравнении прямой r = r.origin + r.direction * t;
	// v -- точка пересечения
	bool trace(const ray_t &r,scalar_t &t,vec_t &v,scalar_t t_min = 0, scalar_t t_max = 1) const {
		if(!trace(r,t,t_min,t_max)) return false;
		v = r.r0 + r.a * t;
		return true;
	}

	// найти пересечение с плоскостью pl
	// pl -- плоскость
	// result -- искомая прямая пересечения
	// если плоскости параллельны -- возвращает false
	bool intersect(const plane_t &pl, ray_t &r) const {
		r.a = vec_t(A,B,C) ^ vec_t(pl.A, pl.B, pl.C);
		vec_t dir2 = r.a * r.a;

		scalar_t invdet;
		if(dir2.z > dir2.y && dir2.z > dir2.x && dir2.z > EPSILON)
		{
			/* then get a point on the XY plane */
			invdet = scalar_t(1) / r.a.z;
			/* solve < pl1.x * xpt.x + pl1.y * xpt.y = - pl1.w >
			< pl2.x * xpt.x + pl2.y * xpt.y = - pl2.w > */
			r.r0.x = B * pl.D - pl.B *D;
			r.r0.y = pl.A * D - A * pl.D;
			r.r0.z = 0;
		}
		else if (dir2.y > dir2.x && dir2.y > EPSILON)
		{
			/* then get a point on the XZ plane */
			invdet = scalar_t(1) / r.a.y;
			/* solve < pl1.x * xpt.x + pl1.z * xpt.z = -pl1.w >
			< pl2.x * xpt.x + pl2.z * xpt.z = -pl2.w > */
			r.r0.x = C * pl.D - pl.C * D;
			r.r0.y = 0;
			r.r0.z = pl.A * D -A * pl.D;
		}
		else if (dir2.x > EPSILON)
		{
			/* then get a point on the YZ plane */
			invdet = scalar_t(1) / r.a.x;
			/* solve < pl1.y * xpt.y + pl1.z * xpt.z = - pl1.w >
			< pl2.y * xpt.y + pl2.z * xpt.z = - pl2.w > */
			r.r0.x = 0;
			r.r0.y = C * pl.D - pl.C * D;
			r.r0.z = pl.B * D -B * pl.D;
		}
		else return false;

		r.r0 *= invdet;

		invdet = scalar_t(1) / sqrt(dir2.x + dir2.y + dir2.z);
		r.a *= invdet;

		return true;
	}
};

void bind_plane(lua_State *L);

}
