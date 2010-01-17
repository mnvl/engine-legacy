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
#include "shape.h"

namespace spatial
{

Shape::~Shape()
{
}

void Shape::bind(lua_State *L)
{
	using namespace luabind;

	module(L, "spatial")
	[
		class_<Shape, ShapePtr>("Shape")
		.def("get_bounds", &Shape::get_bounds)
		.def("intersects", (bool (Shape::*)(Shape const *) const) &Shape::intersects)
	];
}

}
