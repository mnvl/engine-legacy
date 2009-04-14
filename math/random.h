#pragma once

#include <engine/math/scalar.h>
#include <engine/math/vec.h>

namespace math {
	class random;
}

class math::random {
public:
	typedef scalar scalar_type;

	// конструктор и деструктор
	random();
	~random();

	// максимальные значения для частей зерна
	static const int ij_max = 31328;
	static const int kl_max = 30081;

	// устанавливает зерно для генератора случайных чисел, параметры
	// ij, kl -- значения частей зерна; 0<ij<ij_max, 0<kl<kl_max
	void set_seed(int ij,int kl);

	// вычисляет случайное число в интервале от 0 до 1
	scalar_type get_scalar() {
		scalar_type uni;

		uni = u[i97] - u[j97];
		if (uni < 0.0) uni += 1.0;
		u[i97] = uni;
		i97--;
		if (i97==0) i97 = 97;
		j97--;
		if (j97==0) j97 = 97;
		c -= cd;
		if (c<0.0) c += cm;
		uni -= c;
		if (uni<0.0) uni += 1.0;

		return uni;
	}

	scalar_type get_scalar(scalar_type min,scalar_type max) {
		return min + (max - min) * get_scalar();
	}

	template<int N> math::vec<N> get_vector(scalar_type min,scalar_type max) {
		math::vec<N,scalar_type> result;
		for(int i=0; i<N; i++) result.i[i] = get_scalar(min,max);
		return result;
	}

private:
	scalar_type u[98], c, cd, cm;
	int i97, j97;
};

