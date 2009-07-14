#pragma once

#include <math/scalar.h>
#include <math/vec.h>
#include <math/ray.h>

namespace math {

template<int N,class T=scalar>
class line {
public:
    typedef size_t size_type;
    static const size_type arity = N;
    typedef T scalar_type;
    typedef vec<arity, scalar_type> vec_type;
    typedef ray<arity, scalar_type> ray_type;
    typedef line<arity, scalar_type> line_type;

    vec_type A, B;

    line() {
    }

    line(const vec_type &a,const vec_type &b) {
        construct(a,b);
    }

    void construct(const vec_type &a,const vec_type &b) {
        A=a;
        B=b;
    }

    vec_type direction() const {
        return B - A;
    }

    ray_type get_ray() const {
        return ray_type(A, direction());
    }

    scalar_type length_sq() const {
        return direction().length_sq();
    }
};

}

