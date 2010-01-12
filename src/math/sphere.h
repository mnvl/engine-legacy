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

#include <math/scalar.h>
#include <math/vec.h>
#include <math/aabb.h>
#include <math/ray.h>
#include <math/line.h>

namespace math {
	template<int N,class T=scalar>
	class sphere;
}

template<int N,class T>
class math::sphere {
public:
	typedef size_t size_t;
	static const size_t ARITY = N;
	typedef T scalar_t;
	typedef std::numeric_limits<scalar_t> limits_t;
	typedef vec<ARITY,scalar_t> vec_t;
	typedef aabb<ARITY,scalar_t> aabb_t;
	typedef ray<ARITY,scalar_t> ray_t;
	typedef line<ARITY,scalar_t> line_t;
	typedef sphere<ARITY,scalar_t> sphere_t;

	vec_t centre;
	scalar_t radius;

	sphere()
	{
	}

	sphere(const aabb_t &a)
	{
		set(a);
	}

	sphere(const vec_t &c, scalar_t r):
		centre(c),
		radius(r)
	{
	}

	void construct(const aabb_t &a)
	{
		centre = (a.min + a.max) / 2;
		radius = (a.max - centre).length();
	}

	void set(const vec_t &c, scalar_t r)
	{
		centre = c;
		radius = r;
	}

	aabb_t get_aabb() const {
		vec_t r;
		r.set_all(radius / 2);
		return aabb_t(centre - r, centre + r);
	}

	bool contains(const vec_t &v) const {
		return (v - centre).length_sq() < radius * radius;
	}

	bool intersects(const sphere_t &sph) const {
		return (centre - sph.centre).length() < radius + sph.radius;
	}

	// проверить на пересечение с лучом
	bool intersects(const ray_t &r) const {
		// |x-r0|=R
		// x=x0+a*t
		// |x0+a*t-r0|=R
		// (a*t+x0-r0,a*t+x0-r0)=R^2
		// (a*t,a*t+x0-r0)+(x0-r0,a*t+x0-r0)=R^2
		// (a,a)*t^2+(a,x0-r0)*t+(a,x0-r0)*t+(x0-r0,x0-r0)=R^2
		// (a,a)*t^2+(a,x0-r0)*2*t+(x0-r0,x0-r0)-R^2=0
		// A=(a,a)
		// B=(a,x0-r0)*2
		// C=(x0-r0,x0-r0)-R

		scalar_t A = r.a.length_sq();
		scalar_t B = 2 * (r.a & (r.r0 - centre));
		scalar_t C = (r.r0 - centre).length_sq() - radius * radius;

		return B * B - 4 * A * C >= 0;
	}

	// найти точки пересечени€ с лучом (пр€мой) 
	// r -- луч
	// v1, v2 -- точки пересечени€
	bool intersect(const ray_t &r, vec_t &v1, vec_t &v2) const {
		vec_t delta = r.r0 - centre;
		scalar_t A = r.a.length_sq();
		scalar_t B = 2 * (r.a & delta);
		scalar_t C = delta.length_sq() - radius * radius;

		scalar_t discriminant = B * B - 4 * A * C;
		if(discriminant < 0) return false;

		if(abs(discriminant) < EPSILON) {
			v1 = v2 = r.r0 + r.a * (-B / (2 * A));
		} else {
			scalar_t discriminant_sqrt = sqrt(discriminant);
			v1 = r.r0 + r.a * ((-B + discriminant_sqrt) / (2 * A));
			v2 = r.r0 + r.a * ((-B - discriminant_sqrt) / (2 * A));
		}

		return true;
	}
};

