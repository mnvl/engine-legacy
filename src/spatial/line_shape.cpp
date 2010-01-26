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
#include "sphere_shape.h"
#include "line_shape.h"

namespace spatial
{

namespace
{
	ShapePtr upcast_to_shape(boost::shared_ptr<LineShape> const &shape_ptr)
	{
		return boost::static_pointer_cast<Shape>(shape_ptr);
	}
}

LineShape::LineShape()
{
}

LineShape::LineShape(math::vec<3> const &A, math::vec<3> const &B):
	line(A, B)
{
}

LineShape::~LineShape()
{
}

math::aabb<3> LineShape::get_bounds() const
{
	return line::get_aabb();
}

bool LineShape::intersects(Shape const *shape) const
{
	return shape->intersects(this);
}

bool LineShape::intersects(SphereShape const *shape) const
{
	return math::intersects(*shape, *this);
}

bool LineShape::intersects(LineShape const *shape) const
{
	return math::intersects(*this, *shape);
}

bool LineShape::intersects(CapsuleShape const *shape) const
{
	// NOT IMPLEMENTED
	assert(0);
	return false;
}

void LineShape::bind(lua_State *L)
{
	using namespace luabind;

	module(L, "spatial")
	[
		class_<LineShape, Shape, math::line<3>, boost::shared_ptr<LineShape> >("LineShape")
		.def(constructor<math::vec<3> const &, math::vec<3> const &>())
		.scope [ def("upcast_to_shape", &upcast_to_shape) ]
	];
}

}
