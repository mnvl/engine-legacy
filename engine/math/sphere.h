#pragma once

#include <math/scalar.h>
#include <math/vec.h>
#include <math/aabb.h>
#include <math/ray.h>

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

	// найти точки пересечения с лучом (прямой) 
	// r -- луч
	// v1, v2 -- точки пересечения
	bool intersect(const ray_type &r, vec_type &v1, vec_type &v2) const {
		vec_type delta = r.origin - centre;
		scalar_type A = r.direction.length_sq();
		scalar_type B = 2 * (r.direction & delta);
		scalar_type C = delta.length_sq() - radius * radius;

		scalar_type discriminant = B * B - 4 * A * C;
		if(discriminant < 0) return false;

		if(abs(discriminant) < limits_type::epsilon()) {
			v1 = v2 = r.origin + r.direction * (-B / (2 * A));
		} else {
			scalar_type discriminant_sqrt = sqrt(discriminant);
			v1 = r.origin + r.direction * ((-B + discriminant_sqrt) / (2 * A));
			v2 = r.origin + r.direction * ((-B - discriminant_sqrt) / (2 * A));
		}

		return true;
	}

	// найти время через которое сфера столкнется с сферой sph
	// sph -- сфера
	// vel -- относительная скорость sph относительно текущей сферы
	// t -- время, через которое произойдет столкновение
	bool collide(const sphere_type &sph,const vec_type &vel,scalar_type &t) const {
		// |x1 - x2| = R1 + R2
		// x2(t) = x2 + v * t
		// |x1 - x2 + v*t| = R1 + R2
		// (x1 - x2 + v*t, x1 - x2 + v*t) = (R1 + R2)^2
		// (v, v) * t^2 + (v, x1 - x2) * 2 * t + (x1 - x2, x1 - x2) - (R1 + R2)^2 = 0
		// A = (v, v)
		// B = 2 * (v, x1 - x2)
		// C = (x1 - x2, x1 - x2) - (R1 + R2)^2

		scalar_type R = radius + sph.radius;
		vec_type delta = centre - sph.centre;
		scalar_type A = vel.length_sq();
		scalar_type B = 2 * (vel & delta);
		scalar_type C = delta.length_sq() - R * R;

		scalar_type discriminant = B * B - 4 * A * C;
		if(discriminant < 0) return false;

		if(abs(discriminant) < limits_type::epsilon()) {
			t = -B / (2 * A);
			return t >= 0;
		} else {
			scalar_type discriminant_sqrt = sqrt(discriminant);
			scalar_type t1 = (-B + discriminant_sqrt) / (2 * A);
			scalar_type t2 = (-B - discriminant_sqrt) / (2 * A);
			
			if(t1>0) {
				if(t2>0) t = std::min(t1,t2);
				else t = t1;
			} else {
				if(t2>0) t = t2;
				else return false;
			}
		}

		return true;
	}
};

