//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#pragma once

#include <limits>
#include <lua.h>
#include <math/scalar.h>
#include <math/vec.h>

namespace math {

template<const int N,class T=scalar>
class ray {
public:
    static const int ARITY = N;
    typedef T scalar_t;
    typedef vec<ARITY,scalar_t> vec_t;
    typedef ray<ARITY,scalar_t> ray_t;
    typedef std::numeric_limits<scalar_t> limits_t;

    vec_t r0, a;

    ray() {
    }

    ray(const vec_t &origin, const vec_t &direction):
		r0(origin),
		a(direction)
	{
    }

    void set(const vec_t &origin,const vec_t &direction) {
        r0 = origin;
		a = direction;
    }

    vec_t apply(scalar_t t) const
	{
        return r0 + a * t;
    }
};

void bind_ray(lua_State *L);

}

