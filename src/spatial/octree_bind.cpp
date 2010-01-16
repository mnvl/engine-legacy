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
#include "octree.h"

namespace spatial
{

void Octree::bind(lua_State *L)
{
	using namespace luabind;

    module(L, "spatial")
	[
		class_<Octree, boost::shared_ptr<Octree> >("Octree")
		.def(constructor<Octree::aabb_t>())
		.def("add_shape", (void (Octree::*)(ShapePtr const &)) &Octree::add_shape)
		.def("remove_shape", (void (Octree::*)(ShapePtr const &)) &Octree::remove_shape)
		.def("pre_move_shape", &Octree::pre_move_shape)
		.def("post_move_shape", &Octree::post_move_shape)
		.def("intersect", (void (Octree::*)(ShapePtr const &, Octree::shapes_t &)) &Octree::intersect)
	];
}

}
