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

#include <vector>
#include <luabind/lua_include.hpp>
#include <math/vec.h>
#include <math/aabb.h>
#include "shape.h"
#include "result_set.h"

namespace spatial
{

// 2-dimensional cell space partitioning
class CSP
{
public:
	CSP(math::aabb<3> const &bounds, math::scalar cell_side);
	~CSP();

	void add_shape(ShapePtr const &shape, aabb_t const &bounds, ResultSetPtr const &result_set = ResultSetPtr());
	void remove_shape(ShapePtr const &shape, aabb_t const &bounds);
	void move_shape(ShapePtr const &shape, aabb_t const &old_bounds, aabb_t const &new_bounds, ResultSetPtr const &result_set = ResultSetPtr());
	void intersect(ShapePtr const &shape, aabb_t const &bounds, ResultSetPtr const &result); // does not clear result

	void add_shape(ShapePtr const &shape)
		{ add_shape(shape, shape->get_bounds(), ResultSetPtr()); }
	void add_shape(ShapePtr const &shape, ResultSetPtr const &result_set)
		{ add_shape(shape, shape->get_bounds(), result_set); }
	void remove_shape(ShapePtr const &shape)
		{ remove_shape(shape, shape->get_bounds()); }
	void pre_move_shape(ShapePtr const &shape, aabb_t const &new_bounds)
		{ move_shape(shape, shape->get_bounds(), new_bounds); }
	void pre_move_shape(ShapePtr const &shape, aabb_t const &new_bounds, ResultSetPtr const &result_set)
		{ move_shape(shape, shape->get_bounds(), new_bounds, result_set); }
	void post_move_shape(ShapePtr const &shape, aabb_t const &old_bounds)
		{ move_shape(shape, old_bounds, shape->get_bounds()); }
	void post_move_shape(ShapePtr const &shape, aabb_t const &old_bounds, ResultSetPtr const &result_set)
		{ move_shape(shape, old_bounds, shape->get_bounds(), result_set); }
	void intersect(ShapePtr const &shape, ResultSetPtr const &result)
		{ intersect(shape, shape->get_bounds(), result); }

	static void bind(lua_State *L);
};

}
