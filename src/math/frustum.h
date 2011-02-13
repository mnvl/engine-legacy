#pragma once
//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2011 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include "vec.h"
#include "plane.h"
#include "matrix.h"

namespace math
{

template<int N, class T> class aabb;
template<int N, class T> class obb;

// this class is specialized for frustum culling
template<class T = scalar>
class frustum {
public:
	typedef T scalar_t;
	static size_t const ARITY = 3;
	static size_t const PLANES_COUNT = 6;
	typedef plane<scalar_t> plane_t;

	enum {
		PLANE_LEFT,
		PLANE_RIGHT,
		PLANE_TOP,
		PLANE_BOTTOM,
		PLANE_NEAR,
		PLANE_FAR,
	};

	plane_t planes[PLANES_COUNT];

	frustum();
	frustum(math::matrix<4,4> const &tf);
	~frustum();

	void load(math::matrix<4,4> const &tf);

	bool contains(math::vec<3> const &point) const;
	bool test_intersection(aabb<3,T> const &bounds) const;
	bool test_intersection(obb<3,T> const &bounds) const;
};

}
