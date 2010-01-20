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

#include <math/sphere.h>
#include "shape.h"

namespace spatial
{

class SphereShape: public Shape, public math::sphere<3>
{
public:
	SphereShape();
	SphereShape(math::vec<3> const &centre, math::scalar radius);
	virtual ~SphereShape();

	virtual math::aabb<3> get_bounds() const;

	virtual bool intersects(Shape const *shape) const;
	virtual bool intersects(SphereShape const *shape) const;

	void set_centre(math::vec<3> const &c)
	{
		centre = c;
	}

	void set_radius(math::scalar r)
	{
		radius = r;
	}

	static void bind(lua_State *L);
};

}
