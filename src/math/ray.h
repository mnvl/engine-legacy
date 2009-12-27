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

    vec_type origin, direction;

    ray() {
    }

    ray(const vec_type &r0,const vec_type &a):
		origin(r0),
		direction(a)
	{
    }

    void construct(const vec_type &r0,const vec_type &a) {
        origin = r0;
		direction = a;
    }

    vec_type apply(scalar_type t) const {
        return origin + direction * t;
    }
};

void bind_ray(lua_State *L);

}

