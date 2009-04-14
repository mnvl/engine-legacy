#pragma once


#include <engine/math/scalar.h>
#include <engine/math/vec.h>
#include <engine/math/aabb.h>
#include <engine/math/ray.h>
#include <engine/math/line.h>

namespace math {
	template<int N,class T=scalar>
	class sphere;
}

template<int N,class T>
class math::sphere {
public:
	typedef size_t size_type;
	static const size_type arity = N;
	typedef T scalar_type;
	typedef std::numeric_limits<scalar_type> limits_type;
	typedef vec<arity,scalar_type> vec_type;
	typedef aabb<arity,scalar_type> aabb_type;
	typedef ray<arity,scalar_type> ray_type;
	typedef line<arity,scalar_type> line_type;
	typedef sphere<arity,scalar_type> sphere_type;

	vec_type centre;
	scalar_type radius;

	sphere() {
	}

	sphere(const aabb_type &a) {
		construct(a);
	}

	sphere(const vec_type &c, scalar_type r):
		centre(c),
		radius(r)
	{
	}

	void construct(const aabb_type &a) {
		centre = (a.min + a.max) / 2;
		radius = (a.max - centre).length();
	}

	void construct(const vec_type &c, scalar_type r) {
		centre = c;
		radius = r;
	}

	aabb_type get_aabb() const {
		vec_type r;
		r.set_all(radius / 2);
		return aabb_type(centre - r, centre + r);
	}

	bool contains(const vec_type &v) const {
		return (v - centre).length_sq() < radius * radius;
	}

	bool intersects(const sphere_type &sph) const {
		return (centre - sph.centre).length() < radius + sph.radius;
	}

	// проверить на пересечение с лучом
	bool intersects(const ray_type &r) const {
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

		scalar_type A = r.direction.length_sq();
		scalar_type B = 2 * (r.direction & (r.origin - centre));
		scalar_type C = (r.origin - centre).length_sq() - radius * radius;

		return B * B - 4 * A * C >= 0;
	}

	// найти точки пересечени€ с лучом (пр€мой) 
	// r -- луч
	// v1, v2 -- точки пересечени€
	bool intersect(const ray_type &r, vec_type &v1, vec_type &v2) const {
		vec_type delta = r.origin - centre;
		scalar_type A = r.direction.length_sq();
		scalar_type B = 2 * (r.direction & delta);
		scalar_type C = delta.length_sq() - radius * radius;

		scalar_type discriminant = B * B - 4 * A * C;
		if(discriminant < 0) return false;

		if(abs(discriminant) < EPSILON) {
			v1 = v2 = r.origin + r.direction * (-B / (2 * A));
		} else {
			scalar_type discriminant_sqrt = sqrt(discriminant);
			v1 = r.origin + r.direction * ((-B + discriminant_sqrt) / (2 * A));
			v2 = r.origin + r.direction * ((-B - discriminant_sqrt) / (2 * A));
		}

		return true;
	}
};

