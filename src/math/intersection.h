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
#include "frustum.h"

namespace math
{

template<class T> inline bool
query_intersection(ray<3, T> const &ray1, ray<3, T> const &ray2, T &t)
{
	vec<3,T> a1_x_a2 = ray1.a ^ ray2.a;
	T a1_x_a2_len_sq = a1_x_a2.length_sq();

	if (a1_x_a2_len_sq < EPSILON) return false;

	matrix<3,3> m;
	m.assign(ray2.r0 - ray1.r0, ray2.a, a1_x_a2);
	t = m.determinant() / a1_x_a2_len_sq;
	return true;
}

template<class T> inline bool
query_intersection(ray<2, T> const &r1, ray<2, T> const &r2, T &s)
{
	return query_intersection(ray<3,T>(r1.r0, r1.a), ray<3,T>(r2.r0, r2.a), s);
}

template<int N, class T> inline bool
test_intersection(line<N, T> const &l1, line<N, T> const &l2)
{
	T s;
	return query_intersection(l1.get_ray(), l2.get_ray(), s) && s > 0 && s < 1;
}

}

