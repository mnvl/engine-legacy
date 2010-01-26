#pragma once
//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include "scalar.h"
#include "vec.h"
#include "sphere.h"
#include "ray.h"
#include "line.h"
#include "capsule.h"
#include "matrix.h"
#include "aabb.h"
#include "obb.h"

namespace math
{

template<int N, class T> bool inline
intersects(sphere<N, T> const &s, sphere<N, T> const &t)
{
	return (s.centre - t.centre).length_sq() <= square(s.radius + t.radius);
}

// проверить на пересечение с лучом
template<int N, class T> bool inline
intersects(sphere<N, T> const &s, ray<N, T> const &r)
{
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

	T A = r.a.length_sq();
	T B = 2 * (r.a & (r.r0 - s.centre));
	T C = (r.r0 - s.centre).length_sq() - s.radius * s.radius;

	return B * B - 4 * A * C >= 0;
}

template<int N, class T> int inline
intersect(sphere<N, T> const &s, ray<N, T> const &r, vec<N, T> &p1, vec<N, T> &p2)
{
	// p = r.direction * t + r.origin
	// |p - s.centre| ^ 2 = s.radius ^ 2
	// |r.direction * t + r.origin - s.centre| ^ 2 = s.radius ^ 2
	// delta = r.origin - s.centre
	// r.direction.length_sq() * t + 2 * (r.direction & delta) * t + delta.length_sq() - s.radius ^ 2 = 0

	vec<N, T> delta = r.r0 - s.centre;
	T A = r.a.length_sq();
	T B = T(2) * (r.a & delta);
	T C = delta.length_sq() - square(s.radius);

	T discriminant = square(B) - 4 * A * C;

	if (discriminant < 0) {
		return false;
	}

	if (discriminant < EPSILON) {
		T t = -B / (2 * A);
		p1 = p2 = r.a * t + r.r0;
		return 1;
	}

	T discriminant_sqrt = sqrt(discriminant);

	T t1 = (-B + discriminant_sqrt) / (2 * A);
	p1 = r.a * t1 + r.r0;

	T t2 = (-B - discriminant_sqrt) / (2 * A);
	p2 = r.a * t1 + r.r0;

	return 2;
}

template<int N, class T> bool inline
intersects(sphere<N, T> const &s, line<N, T> const &l)
{
	vec<N, T> p1, p2;

	T ll = l.length_sq();

	switch(intersect(s, l.get_ray(), p1, p2)) {
		case 0: return false;
		case 1: return (p1 - l.A).length_sq() < ll;
		default: return (p1 - l.A).length_sq() < ll || (p2 - l.A).length_sq() < ll;
	}
}

template<int N, class T> int inline
intersect(sphere<N, T> const &s, line<N, T> const &l, vec<N, T> &p1, vec<N, T> &p2) {
	switch(intersect(s, l.get_ray(), p1, p2)) {
		case 0: return 0;
		case 1: return int(l.contains(p1));
		default: return int(l.contains(p1)) + int(l.contains(p2));
	}
}

template<class T> bool inline
intersects(ray<3, T> const &r1, ray<3, T> const &r2)
{
	return r1.distance(r2) < EPSILON;
}

template<class T> bool
intersect(ray<3, T> const &ray1, ray<3, T> const &ray2, T &t)
{
	vec<3,T> a1_x_a2 = ray1.a ^ ray2.a;
	T a1_x_a2_len_sq = a1_x_a2.length_sq();

	if (a1_x_a2_len_sq < EPSILON) return false;

	matrix<3,3> m;
	m.assign(ray2.r0 - ray1.r0, ray2.a, a1_x_a2);
	t = m.determinant() / a1_x_a2_len_sq;
	return true; 
}

template<class T> bool inline
intersect(ray<2, T> const &r1, ray<2, T> const &r2, T &s)
{
	return intersect(ray<3,T>(r1.r0, r1.a), ray<3,T>(r2.r0, r2.a), s);
}

template<int N, class T> bool inline
intersects(line<N, T> const &l1, line<N, T> const &l2)
{
	T s;
	return intersect(l1.get_ray(), l2.get_ray(), s) && s > 0 && s < 1;
}

template<class T> bool inline
intersects(sphere<3,T> const &s, capsule<T> const &c)
{
	return c.distance(s.centre) <= s.radius;
}

}

