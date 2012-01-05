//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2012 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include "shape.h"
#include "sphere_shape.h"
#include "line_shape.h"
#include "capsule_shape.h"
#include "result_set.h"
#include "octree.h"
#include "bind.h"

namespace spatial
{

void bind(lua_State *L)
{
	Shape::bind(L);
	SphereShape::bind(L);
	LineShape::bind(L);
	CapsuleShape::bind(L);
	ResultSet::bind(L);
	Octree::bind(L);
}

}
