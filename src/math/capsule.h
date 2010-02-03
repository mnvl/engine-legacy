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

#include <luabind/luabind.hpp>
#include "line.h"

namespace math
{

template<int N, class T> class aabb;
template<int N, class T> class obb;

template<int N, class T = scalar>
class capsule {
public:
    static const size_t ARITY = N;
    typedef T scalar_t;
	typedef vec<ARITY, scalar_t> vec_t;
	typedef line<ARITY, scalar_t> line_t;

	line_t axe;
	scalar_t radius;

    capsule()
	{
    }

    capsule(vec_t const &a, vec_t const &b, scalar_t r):
		axe(a, b),
		radius(r)
	{
    }

	bool contains(vec_t const &point) const
	{
		return axe.distance(point) <= radius;
	}

	scalar_t distance(vec_t const &point) const
	{
		scalar_t d = axe.distance(point);
		if (d <= radius) return 0;
		return d - radius;
	}

	void get_aabb(aabb<ARITY, scalar_t> &bounds) const;
};

void bind_capsule(lua_State *L);

}

