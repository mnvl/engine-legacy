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

#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <lua.h>
#include <math/aabb.h>

namespace spatial
{

class Shape;
class SphereShape;

typedef boost::weak_ptr<Shape> ShapeWeakPtr;
typedef boost::shared_ptr<Shape> ShapePtr;

class Shape
{
public:
	virtual ~Shape();
	virtual math::aabb<3> get_bounds() const = 0;

	virtual bool intersects(Shape const *shape) const = 0;
	virtual bool intersects(SphereShape const *shape) const = 0;

	static void bind(lua_State *L);
};

}
