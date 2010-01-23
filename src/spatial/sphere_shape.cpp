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

namespace spatial
{

namespace
{
	ShapePtr upcast_to_shape(boost::shared_ptr<SphereShape> const &shape_ptr)
	{
		return boost::static_pointer_cast<Shape>(shape_ptr);
	}
}

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
	return math::intersects(*this, *shape);
}

bool SphereShape::intersects(LineShape const *shape) const
{
	return math::intersects((math::line<3> const &) *shape, (math::sphere<3> const &) *this);
}

void SphereShape::bind(lua_State *L)
{
	using namespace luabind;

	module(L, "spatial")
	[
		class_<SphereShape, Shape, math::sphere<3>, boost::shared_ptr<SphereShape> >("SphereShape")
		.def(constructor<math::vec<3> const &, math::scalar>())
		.def("set_centre", &SphereShape::set_centre)
		.def("set_radius", &SphereShape::set_radius)
		.scope [ def("upcast_to_shape", &upcast_to_shape) ]
	];
}

}
