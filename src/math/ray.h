#pragma once

#include <limits>
#include <lua.h>
#include <math/scalar.h>
#include <math/vec.h>

namespace math {

template<const int N,class T=scalar>
class ray {
public:
    static const int arity = N;
    typedef T scalar_type;
    typedef vec<arity,scalar_type> vec_type;
    typedef ray<arity,scalar_type> ray_type;
    typedef std::numeric_limits<scalar_type> limits_type;

    vec_type r0, a;

    ray() {
    }

    ray(const vec_type &origin, const vec_type &direction):
		r0(origin),
		a(direction)
	{
    }

    void set(const vec_type &origin,const vec_type &direction) {
        r0 = origin;
		a = direction;
    }

    vec_type apply(scalar_type t) const
	{
        return r0 + a * t;
    }
};

void bind_ray(lua_State *L);

}

