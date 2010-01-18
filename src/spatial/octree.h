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

#include <set>
#include <vector>
#include <lua.h>
#include <math/vec.h>
#include <math/aabb.h>
#include "shape.h"
#include "result_set.h"

namespace spatial
{

class Octree
{
public:
	static size_t const SUBCLUSTERS_COUNT = 8;
	typedef math::aabb<3> aabb_t;

	Octree(aabb_t const &bounds);
	~Octree();

	aabb_t get_subcluster_bounds(unsigned index) const;
	unsigned get_subcluster_by_point(math::vec<3> const &point) const;
	size_t get_subclusters_by_aabb(aabb_t const &bounds, unsigned subclusters[SUBCLUSTERS_COUNT]) const;
	unsigned get_subclusters_mask_by_aabb(aabb_t const &bounds) const;

	bool is_leaf() const;

	void add_shape(ShapePtr const &shape, aabb_t const &bounds);
	void remove_shape(ShapePtr const &shape, aabb_t const &bounds);
	void move_shape(ShapePtr const &shape, aabb_t const &old_bounds, aabb_t const &new_bounds);
	void intersect(ShapePtr const &shape, aabb_t const &bounds, ResultSetPtr const &result); // does not clear result

	void add_shape(ShapePtr const &shape) { add_shape(shape, shape->get_bounds()); }
	void remove_shape(ShapePtr const &shape) { remove_shape(shape, shape->get_bounds()); }
	void pre_move_shape(ShapePtr const &shape, aabb_t const &new_bounds) { move_shape(shape, shape->get_bounds(), new_bounds); }
	void post_move_shape(ShapePtr const &shape, aabb_t const &old_bounds) { move_shape(shape, old_bounds, shape->get_bounds()); }
	void intersect(ShapePtr const &shape, ResultSetPtr const &result) { intersect(shape, shape->get_bounds(), result); }

	void build_subclusters();

	static void bind(lua_State *L);

private:
	static unsigned const X_MASK = 1, Y_MASK = 2, Z_MASK = 4;
	static size_t const LEAF_SHAPES_THRESHOLD = 32;

	typedef std::vector<Octree> subclusters_t;
	typedef std::set<ShapeWeakPtr> shapes_t;

	aabb_t bounds_;
	math::vec<3> centre_;

	shapes_t shapes_;
	subclusters_t subclusters_;
};

}
