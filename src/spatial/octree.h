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

#include <math/vec.h>
#include <math/aabb.h>
#include "shape.h"

namespace spatial
{

class Octree
{
public:
	Octree(math::aabb<3> const &bounds);

	math::aabb<3> get_subcluster_bounds(unsigned index);
	unsigned get_subcluster_by_point(math::vec<3> const &point);

private:
	static size_t const X_MASK = 1, Y_MASK = 2, Z_MASK = 4;

	math::aabb<3> bounds_;
	math::vec<3> centre_;
};

}
