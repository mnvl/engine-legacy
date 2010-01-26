//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include "vec.h"

namespace math
{

template class vec<2>;
template class vec<3>;
template class vec<4>;

vec<3> const vec<3>::I(1, 0, 0);
vec<3> const vec<3>::J(0, 1, 0);
vec<3> const vec<3>::K(0, 0, 1);
vec<3> const vec<3>::null(0, 0, 0);

template<class T> void
vec<2,T>::to_polar(scalar_t &modulus,scalar_t &angle) const {
	modulus = length();
	angle = atan2(y, x);
}

template<class T> void
vec<3,T>::to_axis_angle(vec_t &axis, scalar_t &angle) const {
	const vec_t original(1,0,0);

	axis = original ^ *this;
	axis.normalize();

	vec_t basis_y = axis ^ original;

	vec<2,T> xy;
	xy.x = x / length();
	xy.y = *this & basis_y;

	scalar_t t;
	xy.to_polar(t, angle);
}

template<class T> vec<3,T>
vec<3,T>::perpendicular_xy() const
{
	if (abs(x) + abs(y) < EPSILON) return K;
	return vec(y, -x, 0);
}

template<class T> vec<3,T>
vec<3,T>::perpendicular_yz() const
{
	if (abs(y) + abs(z) < EPSILON) return I;
	return vec(0, z, -y);
}

template<class T> vec<3,T>
vec<3,T>::perpendicular_xz() const
{
	if (abs(x) + abs(z) < EPSILON) return J;
	return vec(z, 0, -x);
}

}
