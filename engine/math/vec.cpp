
#include <precompiled.h>
#include <engine/math/vec.h>

template class math::vec<2>;
template class math::vec<3>;
template class math::vec<4>;

template<class T> void
math::vec<2,T>::to_polar(scalar_type &modulus,scalar_type &angle) const {
	modulus = length();
	angle = atan2(y, x);
}

template<class T> void
math::vec<3,T>::to_axis_angle(vec_type &axis, scalar_type &angle) const {
	const vec_type original(1,0,0);

	axis = original ^ *this;
	axis.normalize();

	vec_type basis_y = axis ^ original;

	vec<2,T> xy;
	xy.x = x / length();
	xy.y = *this & basis_y;

	scalar_type t;
	xy.to_polar(t, angle);
}
