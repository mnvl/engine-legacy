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
#include <math/vec.h>
#include <math/aabb.h>
#include "shape.h"

namespace spatial
{

class Octree
{
public:
	typedef std::set<ShapeWeakPtr> shapes_weak_t;
	typedef std::set<ShapePtr> shapes_t;

	Octree(math::aabb<3> const &bounds);
	~Octree();

	math::aabb<3> get_subcluster_bounds(unsigned index) const;
	unsigned get_subcluster_by_point(math::vec<3> const &point) const;

	// mask & (1 << i) is set if bounds intersects subcluster with index i
	unsigned get_subclusters_mask_by_aabb(math::aabb<3> const &bounds) const;

	bool is_leaf() const;

	void add_shape(ShapePtr const &shape);

	// does not clear result
	void intersect(ShapePtr const &shape, shapes_t &result);

	void build_subclusters();

private:
	static unsigned const X_MASK = 1, Y_MASK = 2, Z_MASK = 4;
	static size_t const SUBCLUSTERS_COUNT = 8;
	static unsigned const ALL_SUBCLUSTERS_MASK = (1u << SUBCLUSTERS_COUNT) - 1;
	static size_t const LEAF_SHAPES_THRESHOLD = 32;

	typedef std::vector<Octree> subclusters_t;

	math::aabb<3> bounds_;
	math::vec<3> centre_;

	shapes_weak_t shapes_;
	subclusters_t subclusters_;
};

}
