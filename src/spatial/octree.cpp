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

Octree::Octree(aabb_t const &bounds):
	bounds_(bounds),
	centre_(bounds.centre())
{
}

Octree::~Octree()
{
}

Octree::aabb_t Octree::get_subcluster_bounds(unsigned index) const
{
	math::vec<3> origin = bounds_.lo;
	math::vec<3> extents = bounds_.diagonal() / math::scalar(2);

	if (index & X_MASK) origin.x += extents.x;
	if (index & Y_MASK) origin.y += extents.y;
	if (index & Z_MASK) origin.z += extents.z;

	return aabb_t(origin, origin + extents);
}

unsigned Octree::get_subcluster_by_point(math::vec<3> const &point) const
{
	unsigned index = 0;

	if (point.x >= centre_.x) index |= X_MASK;
	if (point.y >= centre_.y) index |= Y_MASK;
	if (point.z >= centre_.z) index |= Z_MASK;

	return index;
}

size_t Octree::get_subclusters_by_aabb(aabb_t const &bounds, unsigned subclusters[SUBCLUSTERS_COUNT]) const
{
	unsigned c1 = get_subcluster_by_point(bounds.lo);
	unsigned c2 = get_subcluster_by_point(bounds.hi);

	if (c1 == c2)
	{
		subclusters[0] = c1;
		return 1;
	}

	if (c1 == 0 && c2 == SUBCLUSTERS_COUNT - 1)
	{
		for (size_t i = 0; i < SUBCLUSTERS_COUNT; i++) subclusters[i] = i;
		return SUBCLUSTERS_COUNT;
	}

	unsigned diff = c1 ^ c2;

	unsigned hi = diff & (diff - 1);
	unsigned lo = diff & ~hi;

	assert ((c1 | lo | hi) == c2);

	if (hi)
	{
		subclusters[0] = c1;
		subclusters[1] = c1 | lo;
		subclusters[2] = c1 | hi;
		subclusters[3] = c2;
		return 4;
	}
	else
	{
		subclusters[0] = c1;
		subclusters[1] = c2;
		return 2;
	}
}

unsigned Octree::get_subclusters_mask_by_aabb(aabb_t const &bounds) const
{
	unsigned result = 0;

	unsigned subclusters[SUBCLUSTERS_COUNT];
	size_t nsubclusters = get_subclusters_by_aabb(bounds, subclusters);

	for (size_t i = 0; i < nsubclusters; i++)
	{
		result |= 1u << subclusters[i];
	}

	return result;
}

bool Octree::is_leaf() const
{
	return subclusters_.empty();
}

void Octree::add_shape(ShapePtr const &shape, aabb_t const &bounds)
{
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
		unsigned subclusters[SUBCLUSTERS_COUNT];
		size_t nsubclusters = get_subclusters_by_aabb(bounds, subclusters);

		if (nsubclusters == SUBCLUSTERS_COUNT)
		{
			shapes_.insert(shape);
		}
		else
		{
			for (size_t i = 0; i < nsubclusters; i++)
			{
				subclusters_[subclusters[i]].add_shape(shape, bounds);
			}
		}
	}
}

void Octree::remove_shape(ShapePtr const &shape, aabb_t const &bounds)
{
	if (is_leaf())
	{
		shapes_.erase(shape);
	}
	else
	{
		unsigned subclusters[SUBCLUSTERS_COUNT];
		size_t nsubclusters = get_subclusters_by_aabb(bounds, subclusters);

		if (nsubclusters == SUBCLUSTERS_COUNT)
		{
			shapes_.erase(shape);
		}
		else
		{
			for (size_t i = 0; i < nsubclusters; i++)
			{
				subclusters_[subclusters[i]].remove_shape(shape, bounds);
			}
		}
	}
}

void Octree::move_shape(ShapePtr const &shape, aabb_t const &old_bounds, aabb_t const &new_bounds)
{
	if (is_leaf())
	{
		return;
	}
	else
	{
		unsigned subclusters_old[SUBCLUSTERS_COUNT];
		size_t nsubclusters_old = get_subclusters_by_aabb(old_bounds, subclusters_old);

		unsigned subclusters_new[SUBCLUSTERS_COUNT];
		size_t nsubclusters_new = get_subclusters_by_aabb(new_bounds, subclusters_new);

		if (nsubclusters_old == SUBCLUSTERS_COUNT)
		{
			if (nsubclusters_new == SUBCLUSTERS_COUNT)
			{
				return;
			}
			else
			{
				shapes_.erase(shape);
				add_shape(shape, new_bounds);
				return;
			}
		}
		else
		{
			if (nsubclusters_new == SUBCLUSTERS_COUNT)
			{
				remove_shape(shape, old_bounds);
				shapes_.insert(shape);
				return;
			}
		}

		size_t i = 0, j = 0;

		while (i < nsubclusters_old && j < nsubclusters_new)
		{
			if (subclusters_old[i] < subclusters_new[j])
			{
				// shape moved out of subclusters_old[i]
				subclusters_[subclusters_old[i]].remove_shape(shape, old_bounds);
				++i;
			}
			else if (subclusters_old[i] > subclusters_new[j])
			{
				// shape moved into subclusters_new[j]
				subclusters_[subclusters_new[j]].add_shape(shape, new_bounds);
				++j;
			}
			else
			{
				subclusters_[subclusters_old[i]].move_shape(shape, old_bounds, new_bounds);
				++i;
				++j;
			}
		}

		while (i < nsubclusters_old)
		{
			// shape moved out of subclusters_old[i]
			subclusters_[subclusters_old[i]].remove_shape(shape, old_bounds);
			++i;
		}

		while (j < nsubclusters_new)
		{
			// shape moved into subclusters_new[j]
			subclusters_[subclusters_new[j]].add_shape(shape, new_bounds);
			++j;
		}
	}
}

void Octree::intersect(ShapePtr const &shape, aabb_t const &bounds, ResultSetPtr const &result)
{
	shapes_t::const_iterator it = shapes_.begin();

	while (it != shapes_.end())
	{
		if (ShapePtr sh = it->lock())
		{
			if (sh->get_bounds().intersects(bounds))
			{
				// maybe check here that result does not contain shape

				if (sh->intersects(shape.get()))
				{
					result->insert(sh);
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
		unsigned subclusters[SUBCLUSTERS_COUNT];
		size_t nsubclusters = get_subclusters_by_aabb(bounds, subclusters);

		for (size_t i = 0; i < nsubclusters; i++)
		{
			subclusters_[subclusters[i]].intersect(shape, bounds, result);
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
	shapes_t::iterator it = shapes_.begin();

	while (it != shapes_.end())
	{
		if (ShapePtr sh = it->lock())
		{
			aabb_t bounds = sh->get_bounds();

			unsigned subclusters[SUBCLUSTERS_COUNT];
			size_t nsubclusters = get_subclusters_by_aabb(bounds, subclusters);

			if (nsubclusters == SUBCLUSTERS_COUNT)
			{
				it++;
			}
			else
			{
				for (size_t i = 0; i < nsubclusters; i++)
				{
					subclusters_[subclusters[i]].add_shape(sh, sh->get_bounds());
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
