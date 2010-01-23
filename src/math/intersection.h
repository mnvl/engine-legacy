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
#include "aabb.h"
#include "obb.h"

namespace math
{
	template<int N, class S>
	bool intersect(ray<N, S> const &r, vec<N, S> &pt)
	{
		return (pt - r.r0).is_collinear(r.a);
	}

	template<int N, class S>
	bool intersect(line<N, S> const &l, vec<N, S> &pt)
	{
		vec<N, S> v1 = (pt - l.A);
		vec<N, S> v2 = (l.B - l.A);
		return v1.is_collinear(v2) && v1.length_sq() < v2.length_sq() && (v1 & v2) > 0;
	}

	// Graphic Gems, 1990, ed. Andrew S. Glassner, V.3, p. 304
	template<class S>
	bool intersects(ray<3, S> const &r1, ray<3, S> const &r2)
	{
		vec<3, S> v1_x_v2 = r1.a ^ r2.a;
		S v1_x_v2_len = v1_x_v2.length_sq();
		return v1_x_v2_len > EPSILON;
	}

	template<class S>
	bool intersect(ray<3, S> const &r1, ray<3, S> const &r2, S &s)
	{
		vec<3, S> v1_x_v2 = r1.a ^ r2.a;
		S v1_x_v2_len = v1_x_v2.length_sq();
		if(v1_x_v2_len < EPSILON) return false;
		s = (((r2.r0 - r1.r0) ^ r2.a) & v1_x_v2) / v1_x_v2_len;
		return true;
	}

	template<class S>
	bool intersect(ray<2, S> const &r1, ray<2, S> const &r2, S &s)
	{
		return intersect(ray<3,S>(r1.r0, r1.a), ray<3,S>(r2.r0, r2.a), s);
	}

	template<int N, class S>
	bool intersects(line<N, S> const &l1, line<N, S> const &l2)
	{
		S s;
		return intersect(l1.get_ray(), l2.get_ray(), s) && s > 0 && s < 1;
	}

	template<int N, class S>
	bool intersects(sphere<N, S> const &s, sphere<N, S> const &t)
	{
		return (s.centre - t.centre).length_sq() <= square(s.radius + t.radius);
	}

	// проверить на пересечение с лучом
	template<int N, class S>
	bool intersects(sphere<N, S> const &s, ray<N, S> const &r)
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

		S A = r.a.length_sq();
		S B = 2 * (r.a & (r.r0 - s.centre));
		S C = (r.r0 - s.centre).length_sq() - s.radius * s.radius;

		return B * B - 4 * A * C >= 0;
	}

	template<int N, class S>
	int intersect(ray<N, S> const &r, sphere<N, S> const &s, vec<N, S> &p1, vec<N, S> &p2) {
		// p = r.direction * t + r.origin
		// |p - s.centre| ^ 2 = s.radius ^ 2
		// |r.direction * t + r.origin - s.centre| ^ 2 = s.radius ^ 2
		// delta = r.origin - s.centre
		// r.direction.length_sq() * t + 2 * (r.direction & delta) * t + delta.length_sq() - s.radius ^ 2 = 0

		vec<N, S> delta = r.r0 - s.centre;
		S A = r.a.length_sq();
		S B = S(2) * (r.a & delta);
		S C = delta.length_sq() - square(s.radius);

		S discriminant = square(B) - 4 * A * C;

		if (discriminant < 0) {
			return false;
		}

		if (discriminant < EPSILON) {
			S t = -B / (2 * A);
			p1 = p2 = r.a * t + r.r0;
			return 1;
		}

		S discriminant_sqrt = sqrt(discriminant);

		S t1 = (-B + discriminant_sqrt) / (2 * A);
		p1 = r.a * t1 + r.r0;

		S t2 = (-B - discriminant_sqrt) / (2 * A);
		p2 = r.a * t1 + r.r0;

		return 2;
	}

	template<int N, class S>
	bool intersects(line<N, S> const &l, sphere<N, S> const &s)
	{
		vec<N, S> p1, p2;

		S ll = l.length_sq();

		switch(intersect(l.get_ray(), s, p1, p2)) {
			case 0: return false;
			case 1: return (p1 - l.A).length_sq() < ll;
			default: return (p1 - l.A).length_sq() < ll || (p2 - l.A).length_sq() < ll;
		}
	}

	template<int N, class S>
	int intersect(line<N, S> const &l, sphere<N, S> const &s, vec<N, S> &p1, vec<N, S> &p2) {
		switch(intersect(l.get_ray(), s, p1, p2)) {
			case 0: return 0;
			case 1: return int(intersect(l, p1));
			default: return int(intersect(l, p1)) + int(intersect(l, p2));
		}
	}

	// найти точки пересечени€ с лучом (пр€мой) 
	// r -- луч
	// v1, v2 -- точки пересечени€
	template<int N, class S>
	bool intersect(sphere<N,S> const &s, ray<N,S> const &r, vec<N,S> &v1, vec<N,S> &v2)
	{
		vec<N,S> delta = r.r0 - s.centre;
		S A = r.a.length_sq();
		S B = 2 * (r.a & delta);
		S C = delta.length_sq() - s.radius * s.radius;

		S discriminant = B * B - 4 * A * C;
		if(discriminant < 0) return false;

		if(abs(discriminant) < EPSILON) {
			v1 = v2 = r.r0 + r.a * (-B / (2 * A));
		} else {
			S discriminant_sqrt = sqrt(discriminant);
			v1 = r.r0 + r.a * ((-B + discriminant_sqrt) / (2 * A));
			v2 = r.r0 + r.a * ((-B - discriminant_sqrt) / (2 * A));
		}

		return true;
	}
}

