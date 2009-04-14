#pragma once

#include <engine/math/scalar.h>
#include <engine/math/vec.h>
#include <limits>

namespace math {
	// обобщенная прямая
	// N -- мерность прямой
	// T --скалярный тип
	template<const int N,class T=scalar>
	class ray {
	public:
		static const int arity = N;
		typedef T scalar_type;
		typedef vec<arity,scalar_type> vec_type;
		typedef ray<arity,scalar_type> ray_type;
		typedef std::numeric_limits<scalar_type> limits_type;

		// уравнение прямой: r(t) = origin + direction*t
		vec_type origin, direction;

		// пустой конструктор
		ray() {
		}

		// конструктор
		// r0 -- точка прямой
		// a -- направляющий вектор
		ray(const vec_type &r0,const vec_type &a) {
			construct(r0,a);
		}

		// сконструировать прямую
		// r0 -- точка прямой
		// a -- направляющий вектор
		void construct(const vec_type &r0,const vec_type &a) {
			origin = r0;
			direction = a;
		}

		vec_type apply(scalar_type t) const {
			return origin + direction * t;
		}
	};
}

