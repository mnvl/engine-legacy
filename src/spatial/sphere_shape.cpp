//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include "sphere_shape.h"

namespace spatial
{

SphereShape::SphereShape()
{
}

SphereShape::SphereShape(math::vec<3> const &centre, math::scalar radius):
	sphere(centre, radius)
{
}

SphereShape::~SphereShape()
{
}

math::aabb<3> SphereShape::get_bounds() const
{
	return get_aabb();
}

bool SphereShape::intersects(Shape const *shape) const
{
	return shape->intersects(this);
}

bool SphereShape::intersects(SphereShape const *shape) const
{
	return sphere::intersects(*shape);
}

}
