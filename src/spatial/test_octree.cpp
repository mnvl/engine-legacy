//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <stdlib.h>
#include <boost/test/unit_test.hpp>
#include "sphere_shape.h"
#include "octree.h"

BOOST_AUTO_TEST_SUITE (test_octree)

BOOST_AUTO_TEST_CASE (test_subcluster_bounds)
{
	spatial::Octree octree(math::aabb<3>(math::vec<3>(-1, -1, -1), math::vec<3>(1, 1, 1)));

	{
		math::aabb<3> bounds = octree.get_subcluster_bounds(0);
		BOOST_ASSERT ((bounds.min - math::vec<3>(-1, -1, -1)).length_sq() < math::EPSILON);
		BOOST_ASSERT ((bounds.max - math::vec<3>(0, 0, 0)).length_sq() < math::EPSILON);
	}

	{
		math::aabb<3> bounds = octree.get_subcluster_bounds(1);
		BOOST_ASSERT ((bounds.min - math::vec<3>(0, -1, -1)).length_sq() < math::EPSILON);
		BOOST_ASSERT ((bounds.max - math::vec<3>(1, 0, 0)).length_sq() < math::EPSILON);
	}

	{
		math::aabb<3> bounds = octree.get_subcluster_bounds(2);
		BOOST_ASSERT ((bounds.min - math::vec<3>(-1, 0, -1)).length_sq() < math::EPSILON);
		BOOST_ASSERT ((bounds.max - math::vec<3>(0, 1, 0)).length_sq() < math::EPSILON);
	}

	{
		math::aabb<3> bounds = octree.get_subcluster_bounds(3);
		BOOST_ASSERT ((bounds.min - math::vec<3>(0, 0, -1)).length_sq() < math::EPSILON);
		BOOST_ASSERT ((bounds.max - math::vec<3>(1, 1, 0)).length_sq() < math::EPSILON);
	}

	{
		math::aabb<3> bounds = octree.get_subcluster_bounds(4);
		BOOST_ASSERT ((bounds.min - math::vec<3>(-1, -1, 0)).length_sq() < math::EPSILON);
		BOOST_ASSERT ((bounds.max - math::vec<3>(0, 0, 1)).length_sq() < math::EPSILON);
	}

	{
		math::aabb<3> bounds = octree.get_subcluster_bounds(5);
		BOOST_ASSERT ((bounds.min - math::vec<3>(0, -1, 0)).length_sq() < math::EPSILON);
		BOOST_ASSERT ((bounds.max - math::vec<3>(1, 0, 1)).length_sq() < math::EPSILON);
	}

	{
		math::aabb<3> bounds = octree.get_subcluster_bounds(6);
		BOOST_ASSERT ((bounds.min - math::vec<3>(-1, 0, 0)).length_sq() < math::EPSILON);
		BOOST_ASSERT ((bounds.max - math::vec<3>(0, 1, 1)).length_sq() < math::EPSILON);
	}

	{
		math::aabb<3> bounds = octree.get_subcluster_bounds(7);
		BOOST_ASSERT ((bounds.min - math::vec<3>(0, 0, 0)).length_sq() < math::EPSILON);
		BOOST_ASSERT ((bounds.max - math::vec<3>(1, 1, 1)).length_sq() < math::EPSILON);
	}
}

BOOST_AUTO_TEST_CASE (test_subcluster_by_point)
{
	spatial::Octree octree(math::aabb<3>(math::vec<3>(-10, -100, -1000), math::vec<3>(10, 100, 1000)));

	for (size_t i = 0; i < 1000; i++)
	{
		math::vec<3> v(math::scalar((rand() % 20) - 10), math::scalar((rand() % 200) - 100),
			math::scalar((rand() % 2000) - 1000));

		unsigned index = octree.get_subcluster_by_point(v);
		math::aabb<3> bounds = octree.get_subcluster_bounds(index);

		BOOST_ASSERT (bounds.contains(v));
	}
}

BOOST_AUTO_TEST_CASE (test_subclusters_mask_by_aabb)
{
	spatial::Octree octree(math::aabb<3>(math::vec<3>(-5, -5, -5), math::vec<3>(5, 5, 5)));

	// -2,-2 -1,-2 | +1,-2 +2,-2
    // -2,-1 -1,-1 | +1,-1 +2,-1
	// ------------+------------
    // -2,+1 -1,+1 | +1,+1 +2,+1
    // -2,+1 -1,+2 | +1,+1 +2,+2

	BOOST_ASSERT (octree.get_subclusters_mask_by_aabb(math::aabb<3>(math::vec<3>(-2, -2, -2), math::vec<3>(-1, -1, -1))) == 1);
	BOOST_ASSERT (octree.get_subclusters_mask_by_aabb(math::aabb<3>(math::vec<3>(1, -2, -2), math::vec<3>(2, -1, -1))) == 2);
	BOOST_ASSERT (octree.get_subclusters_mask_by_aabb(math::aabb<3>(math::vec<3>(-2, 1, -2), math::vec<3>(-1, 2, -1))) == 4);
	BOOST_ASSERT (octree.get_subclusters_mask_by_aabb(math::aabb<3>(math::vec<3>(1, 1, -2), math::vec<3>(2, 2, -1))) == 8);

	BOOST_ASSERT (octree.get_subclusters_mask_by_aabb(math::aabb<3>(math::vec<3>(-2, -2, 1), math::vec<3>(-1, -1, 2))) == 16);
	BOOST_ASSERT (octree.get_subclusters_mask_by_aabb(math::aabb<3>(math::vec<3>(1, -2, 1), math::vec<3>(2, -1, 2))) == 32);
	BOOST_ASSERT (octree.get_subclusters_mask_by_aabb(math::aabb<3>(math::vec<3>(-2, 1, 1), math::vec<3>(-1, 2, 2))) == 64);
	BOOST_ASSERT (octree.get_subclusters_mask_by_aabb(math::aabb<3>(math::vec<3>(1, 1, 1), math::vec<3>(2, 2, 2))) == 128);

	BOOST_ASSERT (octree.get_subclusters_mask_by_aabb(math::aabb<3>(math::vec<3>(-2, -2, -2), math::vec<3>(2, -1, -1))) == (1 | 2));
	BOOST_ASSERT (octree.get_subclusters_mask_by_aabb(math::aabb<3>(math::vec<3>(-2, -2, -2), math::vec<3>(-1, 2, -1))) == (1 | 4));
	BOOST_ASSERT (octree.get_subclusters_mask_by_aabb(math::aabb<3>(math::vec<3>(1, -2, -2), math::vec<3>(2, 2, -1))) == (2 | 8));
	BOOST_ASSERT (octree.get_subclusters_mask_by_aabb(math::aabb<3>(math::vec<3>(-2, 1, -2), math::vec<3>(2, 2, -1))) == (4 | 8));

	BOOST_ASSERT (octree.get_subclusters_mask_by_aabb(math::aabb<3>(math::vec<3>(-2, -2, 1), math::vec<3>(2, -1, 2))) == (16 | 32));
	BOOST_ASSERT (octree.get_subclusters_mask_by_aabb(math::aabb<3>(math::vec<3>(-2, -2, 1), math::vec<3>(-1, 2, 2))) == (16 | 64));
	BOOST_ASSERT (octree.get_subclusters_mask_by_aabb(math::aabb<3>(math::vec<3>(1, -2, 1), math::vec<3>(2, 2, 2))) == (32 | 128));
	BOOST_ASSERT (octree.get_subclusters_mask_by_aabb(math::aabb<3>(math::vec<3>(-2, 1, 1), math::vec<3>(2, 2, 2))) == (64 | 128));


	BOOST_ASSERT (octree.get_subclusters_mask_by_aabb(math::aabb<3>(math::vec<3>(-2, -2, -2), math::vec<3>(2, 2, -1))) == (1 | 2 | 4 | 8));
	BOOST_ASSERT (octree.get_subclusters_mask_by_aabb(math::aabb<3>(math::vec<3>(-2, -2, 1), math::vec<3>(2, 2, 2))) == (16 | 32 | 64 | 128));

	BOOST_ASSERT (octree.get_subclusters_mask_by_aabb(math::aabb<3>(math::vec<3>(-2, -2, -2), math::vec<3>(2, 2, 2))) == (1 | 2 | 4 | 8 | 16 | 32 | 64 | 128));
}

BOOST_AUTO_TEST_CASE (test_intersect_correctness_1)
{
	spatial::Octree octree(math::aabb<3>(math::vec<3>(-1, -1, -1), math::vec<3>(1, 1, 1)));

	std::vector<spatial::SphereShape> shapes;
	shapes.reserve(8);

	shapes.push_back(spatial::SphereShape(math::vec<3>(-0.5f, -0.5f, -0.5f), 0.1f));
	octree.add_shape(&shapes.back());

	shapes.push_back(spatial::SphereShape(math::vec<3>( 0.5f, -0.5f, -0.5f), 0.1f));
	octree.add_shape(&shapes.back());

	shapes.push_back(spatial::SphereShape(math::vec<3>(-0.5f,  0.5f, -0.5f), 0.1f));
	octree.add_shape(&shapes.back());

	shapes.push_back(spatial::SphereShape(math::vec<3>( 0.5f,  0.5f, -0.5f), 0.1f));
	octree.add_shape(&shapes.back());

	shapes.push_back(spatial::SphereShape(math::vec<3>(-0.5f, -0.5f,  0.5f), 0.1f));
	octree.add_shape(&shapes.back());

	shapes.push_back(spatial::SphereShape(math::vec<3>( 0.5f, -0.5f,  0.5f), 0.1f));
	octree.add_shape(&shapes.back());

	shapes.push_back(spatial::SphereShape(math::vec<3>(-0.5f,  0.5f,  0.5f), 0.1f));
	octree.add_shape(&shapes.back());

	shapes.push_back(spatial::SphereShape(math::vec<3>( 0.5f,  0.5f,  0.5f), 0.1f));
	octree.add_shape(&shapes.back());

	octree.build_subclusters();

	for (size_t i = 0; i < 100; i++)
	{
		math::vec<3> centre(math::scalar(rand() * 2) / RAND_MAX - 1, math::scalar(rand() * 2) / RAND_MAX - 1,
			math::scalar(rand() * 2) / RAND_MAX - 1);
		math::scalar radius = math::scalar(rand()) / RAND_MAX;

		spatial::SphereShape shape(centre, radius);

		std::set<spatial::Shape const *> result;
		octree.intersect(&shape, result);

		for (size_t j = 0; j < shapes.size(); j++)
		{
			bool p1 = shapes[j].intersects(&shape);
			bool p2 = (result.find(&shapes[j]) != result.end());

			BOOST_ASSERT (p1 == p2);
		}
	}
}

BOOST_AUTO_TEST_CASE (test_intersect_correctness_2)
{
	spatial::Octree octree(math::aabb<3>(math::vec<3>(-1, -1, -1), math::vec<3>(1, 1, 1)));

	static size_t const N = 1000;
	static size_t const M = 100;

	std::vector<spatial::SphereShape> shapes;
	shapes.reserve(N);

	for (size_t i = 0; i < N; i++)
	{
		math::vec<3> centre(math::scalar(rand() * 2) / RAND_MAX - 1, math::scalar(rand() * 2) / RAND_MAX - 1,
			math::scalar(rand() * 2) / RAND_MAX - 1);
		math::scalar radius = math::scalar(rand()) / RAND_MAX;

		shapes.push_back(spatial::SphereShape(centre, radius));
		octree.add_shape(&shapes.back());
	}

	for (size_t i = 0; i < M; i++)
	{
		math::vec<3> centre(math::scalar(rand() * 2) / RAND_MAX - 1, math::scalar(rand() * 2) / RAND_MAX - 1,
			math::scalar(rand() * 2) / RAND_MAX - 1);
		math::scalar radius = math::scalar(rand()) / RAND_MAX;

		spatial::SphereShape shape(centre, radius);

		std::set<spatial::Shape const *> result;
		octree.intersect(&shape, result);

		for (size_t j = 0; j < N; j++)
		{
			bool p1 = shapes[j].intersects(&shape);
			bool p2 = (result.find(&shapes[j]) != result.end());

			BOOST_ASSERT (p1 == p2);
		}
	}
}

BOOST_AUTO_TEST_SUITE_END()
