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

#include <luabind/lua_include.hpp>
#include "scalar.h"
#include "vec.h"
#include "aabb.h"

namespace math
{

template<int N,class T=scalar>
class sphere {
public:
	typedef size_t size_t;
	static const size_t ARITY = N;
	typedef T scalar_t;
	typedef std::numeric_limits<scalar_t> limits_t;
	typedef vec<ARITY,scalar_t> vec_t;
	typedef aabb<ARITY,scalar_t> aabb_t;
	typedef sphere<ARITY,scalar_t> sphere_t;

	vec_t centre;
	scalar_t radius;

	sphere()
	{
	}

	sphere(const aabb_t &a)
	{
		set(a);
	}

	sphere(const vec_t &c, scalar_t r):
		centre(c),
		radius(r)
	{
	}

	void construct(const aabb_t &a)
	{
		centre = (a.lo + a.hi) / 2;
		radius = (a.hi - centre).length();
	}

	void set(const vec_t &c, scalar_t r)
	{
		centre = c;
		radius = r;
	}

	aabb_t get_aabb() const
	{
		vec_t r;
		r.set_all(radius);
		return aabb_t(centre - r, centre + r);
	}

	bool contains(const vec_t &v) const
	{
		return (v - centre).length_sq() < radius * radius;
	}
};

void bind_sphere(lua_State *L);

}