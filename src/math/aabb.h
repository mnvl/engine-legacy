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

#include <iosfwd>
#include <lua.h>
#include "scalar.h"
#include "vec.h"
#include "ray.h"
#include "line.h"

namespace math
{

template<int N,class T> class triangle;

template<int N,class T = scalar>
class aabb {
public:
	// типы и константы
	typedef T scalar_t;
	typedef std::numeric_limits<scalar_t> limits_t;
	typedef size_t size_t;
	static const size_t ARITY = N;
	typedef vec<ARITY,scalar_t> vec_t;
	static const size_t vertices_count = 1 << ARITY;
	static const size_t edges_count = vertices_count + (1 << (ARITY-1));
	typedef line<ARITY,scalar_t> line_t;
	typedef ray<ARITY,scalar_t> ray_t;
	typedef triangle<ARITY, scalar_t> triangle_t;
	typedef aabb<ARITY,scalar_t> aabb_t;

	// данные
	vec_t lo, hi;

	// конструкторы
	aabb()
	{
	}

	aabb(const vec_t &centre):
		lo(centre),
		hi(centre)
	{
	}

	aabb(const vec_t &l,const vec_t &h):
		lo(l),
		hi(h)
	{
	}

	aabb(const vec_t &centre, math::scalar radius):
		lo(centre.x - radius/2, centre.y - radius/2, centre.z - radius/2),
		hi(centre.x + radius/2, centre.y + radius/2, centre.z + radius/2)
	{
	}

	// операции
	void null() {
		for(size_t n=0; n<ARITY; n++) {
			lo.i[n] = limits_t::max();
			hi.i[n] = limits_t::min();
		}
	}

	void forward_vector(vec_t &v,const vec_t &v0) const {
		v.x = v0.x / (hi.x - lo.x);
		v.y = v0.y / (hi.y - lo.y);
		v.z = v0.z / (hi.z - lo.z);
	}

	// получить координаты точки в локальной системе координат
	void forward_point(vec_t &v,const vec_t &v0) const {
		forward_vector(v, v0 - lo);
	}

	// перенести прямую в локальную систему координат
	void forward_ray(ray_t &r,const ray_t &r0) const {
		forward_point(r.r0,r0.r0);
		forward_vector(r.a,r0.a);
	}

	void normalize() {
		minmax(lo,hi);
	}

	vec_t diagonal() const {
		return hi - lo;
	}

	vec_t centre() const {
		return (lo + hi) / scalar_t(2);
	}

	void extend(const vec_t &v) {
		for(size_t n=0; n<ARITY; n++) {
			if(lo.i[n] > v.i[n]) lo.i[n] = v.i[n];
			if(hi.i[n] < v.i[n]) hi.i[n] = v.i[n];
		}
	}

	void extend(const aabb_t &b) {
		extend(b.lo);
		extend(b.hi);
	}

	// пересечения
	bool contains(const vec_t &right) const {
		return lo <= right && right <= hi;
	}

	bool contains(const aabb_t &right) const {
		return contains(right.lo) && contains(right.hi);
	}

	bool contains(const triangle_t &right) const {
		return contains(right.A) && contains(right.B) && contains(right.C);
	}

	bool intersects(const aabb_t &right) const {
		return right.hi >= lo && hi >= right.lo;
	}

	bool trace(const ray_t &r, scalar_t t_min = 0, scalar_t t_max = 1) const {
		// перенести луч в локальную систему координат
		ray_t r1;
		forward_ray(r1, r);

		// пересечение прямой r1 с плоскостьями x=0 и x=1
		if(abs(r1.a.x) > EPSILON) {
			scalar_t dx = 1 / r1.a.x;

			scalar_t t = -r1.r0.x * dx;
			vec_t v;
			if(t >= t_min && t <= t_max) {
				v = r1.r0 + r1.a * t;
				if(v.y>=0 && v.y<=1 && v.z>=0 && v.z<=1) return true;
			}

			t = dx - t;
			if(t >= t_min && t <= t_max) {
				v += r1.a * dx;
				if(v.y>=0 && v.y<=1 && v.z>=0 && v.z<=1) return true;
			}
		}

		// пересечение прямой r1 с плоскостьями y=0 и y=1
		if(abs(r1.a.y) > EPSILON) {
			scalar_t dy = 1 / r1.a.y;

			scalar_t t = -r1.r0.y * dy;
			vec_t v;
			if(t >= t_min && t <= t_max) {
				v = r1.r0 + r1.a * t;
				if(v.x>=0 && v.x<=1 && v.z>=0 && v.z<=1) return true;
			}

			t = dy - t;
			if(t >= t_min && t <= t_max) {
				v += r1.a * dy;
				if(v.x>=0 && v.x<=1 && v.z>=0 && v.z<=1) return true;
			}
		}

		// пересечение прямой r1 с плоскостьями z=0 и z=1
		if(abs(r1.a.z) > EPSILON) {
			scalar_t dz = 1 / r1.a.z;

			scalar_t t = -r1.r0.z * dz;
			vec_t v;
			if(t >= t_min && t <= t_max) {
				v = r1.r0 + r1.a * t;
				if(v.x>=0 && v.x<=1 && v.y>=0 && v.y<=1) return true;
			}

			t = dz - t;
			if(t >= t_min && t <= t_max) {
				v += r1.a * dz;
				if(v.x>=0 && v.x<=1 && v.y>=0 && v.y<=1) return true;
			}
		}

		return false;
	}
};

void bind_aabb(lua_State *L);

}

template<int N, class T>
std::ostream &operator <<(std::ostream &lhs, math::aabb<N,T> &rhs)
{
	return lhs << rhs.lo << " x " << rhs.hi;
}
