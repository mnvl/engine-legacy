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

		// проверить принадлежность точки v прямой r
		// v -- точка
		// возвращаемое значение истинно, только если точка принадлежит прямой
		bool intersect(const vec_type &v) const {
			T k1,k2,k3;
			k1=(v.x-origin.x)*direction.y*direction.z;
			k2=(v.y-origin.y)*direction.x*direction.z;
			k3=(v.z-origin.z)*direction.x*direction.y;
			if(origin.x==0) return k2==k3;
			if(origin.y==0) return k1==k3;
			if(origin.z==0) return k1==k2;
			return k1==k2 && k2==k3;
		}

		// найти точку пересечения двух прямых
		// r -- прямая
		// t1, t2 -- значение переменной t в уравнениях прямых r(t)=a*t+r0, при котором
		// происходит пересечение
		// результат истинен только если пересечение существует
		// Graphic Gems, 1990, ed. Andrew S. Glassner, V.3, p. 304
		bool intersect(const ray_type &r,scalar_type &s) const {
			vec_type v1_x_v2 = direction ^ r.direction;
			scalar_type v1_x_v2_len = v1_x_v2.length_sq();
			if(v1_x_v2_len < limits_type::epsilon()) return false;
			s = (((r.origin - origin) ^ r.direction) & v1_x_v2) / v1_x_v2_len;
			return true;
		}

		// найти точку пересечения двух прямых
		// r -- прямая
		// pos -- точка пересечения
		// результат истинен только если пересечение существует (прямые не параллельны)
		bool intersect(const ray_type &r,vec_type &pt) const {
			scalar_type s;
			if(intersect(r,r,s)) {
				pt = origin + direction * time;
				return true;
			}
			return false;
		}
	};
}

