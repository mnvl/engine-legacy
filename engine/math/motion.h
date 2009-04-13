#pragma once


#include <math/scalar.h>
#include <math/vec.h>
#include <math/matrix.h>

namespace math {
	template<int N,class T=scalar> class motion;
}

template<int N,class T>
class math::motion {
public:
	typedef size_t size_type;
	static const size_type arity = N;
	typedef T scalar_type;
	typedef std::numeric_limits<T> limits_type;
	typedef vec<arity,scalar_type> vec_type;
	typedef matrix<arity+1,arity+1,scalar_type> matrix_type;

	// линейная скорость
	vec_type linear_velocity;

	// угловая скорость
	vec_type angular_velocity;

	// центр, вокруг которого совершается вращение; движется
	// со скоростью linear_velocity
	vec_type rotation_centre;

	// конструктор
	motion() {
	}

	// конструктор
	motion(const vec_type &lv):
		linear_velocity(lv),
		angular_velocity(0,0,0),
		rotation_centre(0,0,0)
	{
	}

	// конструктор
	motion(const vec_type &lv,const vec_type &av,const vec_type &rc):
		linear_velocity(lv),
		angular_velocity(av),
		rotation_centre(rc)
	{
	}

	// вычислить производную функции x(t)
	void get_dxdt(vec_type &dxdt,const vec_type &x0, scalar_type t0) const {
		// dx/dt =  V + [W x (X - c0 - V*t)]
		dxdt = linear_velocity + (angular_velocity ^ (x0 - rotation_centre - linear_velocity * t0));
	}

	// вычислить матрицу трансформации за время t
	void get_transform(matrix_type &tf, scalar_type t) {
		tf.translation(-rotation_centre);
		tf.rotate(angular_velocity * t);
		tf.translate(rotation_centre + linear_velocity * t);
	}
};

