//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <luabind/luabind.hpp>
#include <math/intersection.h>
#include "line_shape.h"
#include "sphere_shape.h"
#include "capsule_shape.h"

namespace spatial
{

namespace
{
	ShapePtr upcast_to_shape(boost::shared_ptr<CapsuleShape> const &shape_ptr)
	{
		return boost::static_pointer_cast<Shape>(shape_ptr);
	}
}

CapsuleShape::CapsuleShape()
{
}

CapsuleShape::CapsuleShape(math::vec<3> const &A, math::vec<3> const &B, math::scalar radius):
	capsule(A, B, radius)
{
}

CapsuleShape::~CapsuleShape()
{
}

math::aabb<3> CapsuleShape::get_bounds() const
{
	math::aabb<3> bounds;
	get_aabb(bounds);
	return bounds;
}

bool CapsuleShape::intersects(Shape const *shape) const
{
	return shape->intersects(this);
}

bool CapsuleShape::intersects(SphereShape const *shape) const
{
	return math::intersects(*shape, *this);
}

bool CapsuleShape::intersects(LineShape const *shape) const
{
	return math::intersects((math::sphere<3> const &) *this, (math::line<3> const &) *shape);
}

bool CapsuleShape::intersects(CapsuleShape const *shape) const
{
	// NOT IMPLEMENTED
	assert(0);
	return false;
}

void CapsuleShape::bind(lua_State *L)
{
	using namespace luabind;

	module(L, "spatial")
	[
		class_<CapsuleShape, Shape, math::capsule<3>, boost::shared_ptr<CapsuleShape> >("CapsuleShape")
		.def(constructor<math::vec<3> const &, math::vec<3> const &, math::scalar>())
		.scope [ def("upcast_to_shape", &upcast_to_shape) ]
	];
}

}
