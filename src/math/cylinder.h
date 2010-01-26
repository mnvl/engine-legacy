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

#include "line.h"

namespace math
{

template<int N, class T> class aabb;
template<int N, class T> class obb;

template<class T = scalar>
class cylinder {
public:
    typedef size_t size_t;
    static const size_t ARITY = 3;
    typedef T scalar_t;
	typedef vec<ARITY, scalar_t> vec_t;
	typedef line<ARITY, scalar_t> line_t;

	line_t axe;
	scalar_t radius;

    cylinder()
	{
    }

    cylinder(vec_t const &a, vec_t const &b, scalar_t r):
		axe(a, b),
		radius(r)
	{
    }
};

}

