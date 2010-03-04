//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <boost/test/unit_test.hpp>
#include <math/intersection.h>

BOOST_AUTO_TEST_SUITE (intersection)

BOOST_AUTO_TEST_CASE (ray_ray_intersection_1)
{
	math::ray<2> r1, r2;
	math::scalar s;

	r1.r0.set(0, 0);
	r1.a.set(100, 500);

	r2.r0.set(1, 0);
	r2.a.set(-1, 3);

	math::query_intersection(r1, r2, s);

	math::vec<2> p1 = r1.r0 + r1.a * s;
	BOOST_ASSERT (r2.contains(p1));

	BOOST_ASSERT (math::abs(s - 0.00375f) < math::EPSILON);
}

// TODO: fix this tests
/*
BOOST_AUTO_TEST_CASE (ray_ray_intersection_2)
{
	using math::random;

	for (int i = 0; i < 1000; i++)
	{
		math::ray<3> r1(math::vec<2>(random(), random()), math::vec<2>(random(), random()));
		math::ray<3> r2(math::vec<2>(random(), random()), math::vec<2>(random(), random()));

		math::scalar s;

		if(math::query_intersection(r1, r2, s))
		{
			math::vec<3> p1 = r1.apply(s);
			BOOST_ASSERT (r2.contains(p1));
		}
	}
}

BOOST_AUTO_TEST_CASE (ray_ray_intersection_3)
{
	using math::random;

	for (int i = 0; i < 1000; i++)
	{
		math::ray<3> r1(math::vec<3>(random(), random(), random()), math::vec<3>(random(), random(), random()));
		math::ray<3> r2(math::vec<3>(random(), random(), random()), math::vec<3>(random(), random(), random()));

		math::scalar s;

		if(math::query_intersection(r1, r2, s))
		{
			math::vec<3> p1 = r1.apply(s);
			BOOST_ASSERT (r2.contains(p1));
		}
	}
}
*/

BOOST_AUTO_TEST_SUITE_END()
