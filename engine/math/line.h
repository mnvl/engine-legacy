#pragma once

#include <math/scalar.h>
#include <math/vec.h>
#include <math/ray.h>

namespace math {
	template<int N,class T=scalar>
	class line {
	public:
		// типы и константы
		typedef size_t size_type;
		static const size_type arity=N;
		typedef T scalar_type;
		typedef vec<arity,scalar_type> vec_type;
		typedef ray<arity,scalar_type> ray_type;
		typedef line<arity,scalar_type> line_type;

		vec_type A,B;

		line() {
		}

		line(const vec_type &a,const vec_type &b) {
			construct(a,b);
		}

		void construct(const vec_type &a,const vec_type &b) {
			A=a;
			B=b;
		}

		ray_type get_ray() const {
			return ray_type(A,B-A);
		}

		bool intersect(const ray_type &r,vec_type &pt) {
			scalar_type t1;
			if(get_ray().intersect(r, t1) && t1>=0 && t1<=1) {
				pt = A + (B-A) * t1;
				return true;
			}
			return false;
		}
	};
}

