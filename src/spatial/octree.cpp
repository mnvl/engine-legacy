//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <assert.h>
#include "octree.h"

namespace spatial
{

Octree::Octree(math::aabb<3> const &bounds):
	bounds_(bounds),
	centre_(bounds.centre())
{
}

Octree::~Octree()
{
}

math::aabb<3> Octree::get_subcluster_bounds(unsigned index) const
{
	math::vec<3> origin = bounds_.min;
	math::vec<3> extents = bounds_.diagonal() / math::scalar(2);

	if (index & X_MASK) origin.x += extents.x;
	if (index & Y_MASK) origin.y += extents.y;
	if (index & Z_MASK) origin.z += extents.z;

	return math::aabb<3>(origin, origin + extents);
}

unsigned Octree::get_subcluster_by_point(math::vec<3> const &point) const
{
	unsigned index = 0;

	if (point.x >= centre_.x) index |= X_MASK;
	if (point.y >= centre_.y) index |= Y_MASK;
	if (point.z >= centre_.z) index |= Z_MASK;

	return index;
}

unsigned Octree::get_subclusters_mask_by_aabb(math::aabb<3> const &bounds) const
{
	unsigned c1 = get_subcluster_by_point(bounds.min);
	unsigned c2 = get_subcluster_by_point(bounds.max);

	if (c1 == c2) return 1u << c1;
	if (c1 == 0 && c2 == SUBCLUSTERS_COUNT - 1) return ALL_SUBCLUSTERS_MASK;

	unsigned diff = c1 ^ c2;

	unsigned hi = diff & (diff - 1);
	unsigned lo = diff & ~hi;

	assert ((c1 | lo | hi) == c2);

	if (hi) return (1u << c1) | (1u << (c1 | lo)) | (1u << (c1 | hi)) | (1u << c2);
	else return (1u << c1) | (1u << c2);
}

bool Octree::is_leaf() const
{
	return subclusters_.empty();
}

void Octree::add_shape(ShapePtr const &shape)
{
	math::aabb<3> const &bounds = shape->get_bounds();

	if (is_leaf())
	{
		shapes_.insert(shape);

		if (shapes_.size() > LEAF_SHAPES_THRESHOLD)
		{
			build_subclusters();
		}
	}
	else
	{
		unsigned mask = get_subclusters_mask_by_aabb(bounds);

		if (mask == ALL_SUBCLUSTERS_MASK)
		{
			shapes_.insert(shape);
		}
		else
		{
			for (size_t i = 0; i < SUBCLUSTERS_COUNT; i++)
			{
				if ((1u << i) & mask)
				{
					subclusters_[i].add_shape(shape);
				}
			}
		}
	}
}

void Octree::intersect(ShapePtr const &shape, shapes_t &result)
{
	math::aabb<3> const &bounds = shape->get_bounds();

	shapes_weak_t::const_iterator it = shapes_.begin();

	while (it != shapes_.end())
	{
		if (ShapePtr sh = it->lock())
		{
			if (sh->get_bounds().intersects(bounds))
			{
				// maybe check here that result does not contain shape

				if (sh->intersects(shape.get()))
				{
					result.insert(sh);
				}
			}

			++it;
		}
		else
		{
			// garbage-collect weak pointers to deleted shapes
			it = shapes_.erase(it);
		}
	}

	if (!is_leaf())
	{
		unsigned mask = get_subclusters_mask_by_aabb(bounds);

		for (size_t i = 0; i < SUBCLUSTERS_COUNT; i++)
		{
			if ((1u << i) & mask)
			{
				subclusters_[i].intersect(shape, result);
			}
		}
	}
}

void Octree::build_subclusters()
{
	// create subclusters
	subclusters_.reserve(SUBCLUSTERS_COUNT);

	for (size_t i = 0; i < SUBCLUSTERS_COUNT; i++)
	{
		subclusters_.push_back(Octree(get_subcluster_bounds(i)));
	}

	// move some shapes to subclusters
	shapes_weak_t::iterator it = shapes_.begin();

	while (it != shapes_.end())
	{
		if (ShapePtr sh = it->lock())
		{
			math::aabb<3> bounds = sh->get_bounds();

			unsigned mask = get_subclusters_mask_by_aabb(bounds);

			if (mask == ALL_SUBCLUSTERS_MASK)
			{
				it++;
			}
			else
			{
				for (size_t i = 0; i < SUBCLUSTERS_COUNT; i++)
				{
					if ((1u << i) & mask)
					{
						subclusters_[i].add_shape(sh);
					}
				}

				it = shapes_.erase(it);
			}
		}
		else
		{
			it = shapes_.erase(it);
		}
	}
}

}
