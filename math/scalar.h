#pragma once

#undef min
#undef max

#include <cmath>
#include <algorithm>

namespace math {
	typedef float scalar;

	static scalar const PI = 3.141592654f;
	static scalar const EPSILON = 1.0e-6f;

	inline scalar sign(scalar s) {
		return s > 0 ? scalar(1) : scalar(-1);
	}

	inline scalar abs(scalar s) {
		return s > 0 ? s : -s;
	}

	inline scalar sin(scalar s) {
		return (scalar) std::sin(s);
	}

	inline scalar cos(scalar s) {
		return (scalar) std::cos(s);
	}

	inline scalar tan(scalar s) {
		return (scalar) std::tan(s);
	}

	inline scalar square(scalar s) {
		return s * s;
	}

	inline scalar cube(scalar s) {
		return s * s * s;
	}

	inline scalar square_root(scalar s) {
		return (scalar) std::sqrt(s);
	}

	template<class T> inline T clamp(T x, T low = 0, T high = 1) {
		if(x < low) x = low;
		if(x > low) x = high;
		return x;
	}

	template<class T> inline T periodic_clamp(T x, T low = -PI, T high = PI) {
		T period = high - low;
		while(x < low) x += period;
		while(x > high) x -= period;
		return x;
	} 
}

