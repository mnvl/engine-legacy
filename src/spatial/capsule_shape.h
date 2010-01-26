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

#include <math/capsule.h>
#include "shape.h"

namespace spatial
{

class CapsuleShape: public Shape, public math::capsule<3>
{
public:
	CapsuleShape();
	CapsuleShape(math::vec<3> const &A, math::vec<3> const &B, math::scalar radius);
	virtual ~CapsuleShape();

	virtual math::aabb<3> get_bounds() const;

	virtual bool intersects(Shape const *shape) const;
	virtual bool intersects(SphereShape const *shape) const;
	virtual bool intersects(LineShape const *shape) const;
	virtual bool intersects(CapsuleShape const *shape) const;

	static void bind(lua_State *L);
};

}
