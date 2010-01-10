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

#include <math/scalar.h>
#include <math/vec.h>
#include <math/ray.h>

namespace math {

template<int N,class T=scalar>
class line {
public:
    typedef size_t size_t;
    static const size_t ARITY = N;
    typedef T scalar_t;
    typedef vec<ARITY, scalar_t> vec_t;
    typedef ray<ARITY, scalar_t> ray_t;
    typedef line<ARITY, scalar_t> line_t;

    vec_t A, B;

    line() {
    }

    line(const vec_t &a,const vec_t &b) {
        construct(a,b);
    }

    void construct(const vec_t &a,const vec_t &b) {
        A=a;
        B=b;
    }

    vec_t direction() const {
        return B - A;
    }

    ray_t get_ray() const {
        return ray_t(A, direction());
    }

    scalar_t length_sq() const {
        return direction().length_sq();
    }
};

}

