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
#include <math/ray.h>

BOOST_AUTO_TEST_SUITE (ray)

BOOST_AUTO_TEST_CASE (ray_point_distance)
{
	math::ray<3> r(math::vec<3>(0, 0, 0), math::vec<3>(1, 0, 0));
	BOOST_ASSERT(math::abs(r.distance_sq(math::vec<3>(0, 1, 0)) - 1) < math::EPSILON);
	BOOST_ASSERT(math::abs(r.distance_sq(math::vec<3>(100, 1, 0)) - 1) < math::EPSILON);
}

BOOST_AUTO_TEST_CASE (ray_ray_distance)
{
	math::ray<3> r1(math::vec<3>(0, 0, 0), math::vec<3>(1, 0, 0));
	math::ray<3> r2(math::vec<3>(100, 1, 0), math::vec<3>(0, 0, 1));

	BOOST_ASSERT(math::abs(r1.distance(r2) - 1) < math::EPSILON);
}

BOOST_AUTO_TEST_SUITE_END()
