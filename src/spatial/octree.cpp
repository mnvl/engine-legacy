//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include "octree.h"

namespace spatial
{

Octree::Octree(math::aabb<3> const &bounds):
	bounds_(bounds),
	centre_(bounds.centre())
{
}

math::aabb<3> Octree::get_subcluster_bounds(unsigned index)
{
	math::vec<3> origin = bounds_.min;
	math::vec<3> extents = bounds_.diagonal() / math::scalar(2);

	if (index & X_MASK) origin.x += extents.x;
	if (index & Y_MASK) origin.y += extents.y;
	if (index & Z_MASK) origin.z += extents.z;

	return math::aabb<3>(origin, origin + extents);
}

unsigned Octree::get_subcluster_by_point(math::vec<3> const &point)
{
	unsigned index = 0;

	if (point.x > centre_.x) index |= X_MASK;
	if (point.y > centre_.y) index |= Y_MASK;
	if (point.z > centre_.z) index |= Z_MASK;

	return index;
}

}
