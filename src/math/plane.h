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
		return abs(A*r.direction.x + B*r.direction.y + C*r.direction.z)>EPSILON
			|| contains(r);
	}

	bool contains(vec_type const &p) const {
		return abs(A * p.x + B * p.y + C * p.z + D) < EPSILON;
	}

	bool contains(const ray_type &r) const {
		return abs(A*r.direction.x + B*r.direction.y + C*r.direction.z) < EPSILON && contains(r.origin);
	}

	bool parallel(const ray_type &r) const {
		return !intersects(r);
	}

	scalar_type trace(const ray_type &r) const {
		scalar_type a_n = A*r.direction.x + B*r.direction.y + C*r.direction.z;
		return -(A*r.origin.x + B*r.origin.y + C*r.origin.z + D) / a_n;
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
		v = r.origin + r.direction * t;
		return true;
	}

	// найти пересечение с плоскостью pl
	// pl -- плоскость
	// result -- искомая прямая пересечения
	// если плоскости параллельны -- возвращает false
	bool intersect(const plane_type &pl, ray_type &r) const {
		r.direction = vec_type(A,B,C) ^ vec_type(pl.A, pl.B, pl.C);
		vec_type dir2 = r.direction * r.direction;

		scalar_type invdet;
		if(dir2.z > dir2.y && dir2.z > dir2.x && dir2.z > EPSILON)
		{
			/* then get a point on the XY plane */
			invdet = scalar_type(1) / r.direction.z;
			/* solve < pl1.x * xpt.x + pl1.y * xpt.y = - pl1.w >
			< pl2.x * xpt.x + pl2.y * xpt.y = - pl2.w > */
			r.origin.x = B * pl.D - pl.B *D;
			r.origin.y = pl.A * D - A * pl.D;
			r.origin.z = 0;
		}
		else if (dir2.y > dir2.x && dir2.y > EPSILON)
		{
			/* then get a point on the XZ plane */
			invdet = scalar_type(1) / r.direction.y;
			/* solve < pl1.x * xpt.x + pl1.z * xpt.z = -pl1.w >
			< pl2.x * xpt.x + pl2.z * xpt.z = -pl2.w > */
			r.origin.x = C * pl.D - pl.C * D;
			r.origin.y = 0;
			r.origin.z = pl.A * D -A * pl.D;
		}
		else if (dir2.x > EPSILON)
		{
			/* then get a point on the YZ plane */
			invdet = scalar_type(1) / r.direction.x;
			/* solve < pl1.y * xpt.y + pl1.z * xpt.z = - pl1.w >
			< pl2.y * xpt.y + pl2.z * xpt.z = - pl2.w > */
			r.origin.x = 0;
			r.origin.y = C * pl.D - pl.C * D;
			r.origin.z = pl.B * D -B * pl.D;
		}
		else return false;

		r.origin *= invdet;

		invdet = scalar_type(1) / sqrt(dir2.x + dir2.y + dir2.z);
		r.direction *= invdet;

		return true;
	}
};

void bind_plane(lua_State *L);

}
