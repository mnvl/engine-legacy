//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2011 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <boost/test/unit_test.hpp>
#include "aabb.h"
#include "obb.h"
#include "plane.h"

BOOST_AUTO_TEST_SUITE(plane)

BOOST_AUTO_TEST_CASE(get_origin_1)
{
	math::plane<> p1(math::vec<3>(1, -3, 5), math::vec<3>(-7, 11, -13));
	math::plane<> p2(p1.get_origin(), p1.get_normal());

	BOOST_REQUIRE(abs(p1.A - p2.A) < math::EPSILON);
	BOOST_REQUIRE(abs(p1.B - p2.B) < math::EPSILON);
	BOOST_REQUIRE(abs(p1.C - p2.C) < math::EPSILON);
	BOOST_REQUIRE(abs(p1.D - p2.D) < math::EPSILON);
}

BOOST_AUTO_TEST_CASE(classify_point_1)
{
	math::plane<> pl(math::vec<3>(0, -1, 0), math::vec<3>(1, 1, 1));

	BOOST_REQUIRE(pl.classify(math::vec<3>(0, -2, 0)) == math::plane<>::NEGATIVE);
	BOOST_REQUIRE(pl.classify(math::vec<3>(0, 0, 0)) == math::plane<>::POSITIVE);
	BOOST_REQUIRE(pl.classify(math::vec<3>(0, -1, 0)) == math::plane<>::INTERSECTS);
}

BOOST_AUTO_TEST_CASE(classify_point_2)
{
	math::plane<> pl(math::vec<3>(0, 0, 0), math::vec<3>(1, -1, 1));

	BOOST_REQUIRE(pl.classify(math::vec<3>(-2, -1.5, -2)) == math::plane<>::NEGATIVE);
	BOOST_REQUIRE(pl.classify(math::vec<3>(-1, -2, -1)) == math::plane<>::INTERSECTS);
}

BOOST_AUTO_TEST_CASE(classify_aabb_1)
{
	math::plane<> pl(math::vec<3>(-1, -1, -1), math::vec<3>(1, 1, 1));

	BOOST_REQUIRE(pl.classify(math::aabb<3>(math::vec<3>(-2, -2, -2), math::vec<3>(-1, -1.5, -1))) == math::plane<>::NEGATIVE);
	BOOST_REQUIRE(pl.classify(math::aabb<3>(math::vec<3>(-2, -2, -2), math::vec<3>(-1, -1, -1))) == math::plane<>::INTERSECTS);
	BOOST_REQUIRE(pl.classify(math::aabb<3>(math::vec<3>(0, 0, 0), math::vec<3>(1, 1, 1))) == math::plane<>::POSITIVE);
	BOOST_REQUIRE(pl.classify(math::aabb<3>(math::vec<3>(-1, -1, -1), math::vec<3>(1, 1, 1))) == math::plane<>::INTERSECTS);
}

BOOST_AUTO_TEST_CASE(classify_aabb_2)
{
	math::plane<> pl(math::vec<3>(-1, -1, -1), math::vec<3>(-1, -1, -1));

	BOOST_REQUIRE(pl.classify(math::aabb<3>(math::vec<3>(-2, -2, -2), math::vec<3>(-1, -1.5, -1))) == math::plane<>::POSITIVE);
	BOOST_REQUIRE(pl.classify(math::aabb<3>(math::vec<3>(-2, -2, -2), math::vec<3>(-1, -1, -1))) == math::plane<>::INTERSECTS);
	BOOST_REQUIRE(pl.classify(math::aabb<3>(math::vec<3>(0, 0, 0), math::vec<3>(1, 1, 1))) == math::plane<>::NEGATIVE);
	BOOST_REQUIRE(pl.classify(math::aabb<3>(math::vec<3>(-1, -1, -1), math::vec<3>(1, 1, 1))) == math::plane<>::INTERSECTS);
}

BOOST_AUTO_TEST_CASE(classify_aabb_3)
{
	math::plane<> pl(math::vec<3>(0, 0, 0), math::vec<3>(1, -1, 1));

	BOOST_REQUIRE(pl.classify(math::aabb<3>(math::vec<3>(0, 4, 0), math::vec<3>(1, 5, 1))) == math::plane<>::NEGATIVE);
	BOOST_REQUIRE(pl.classify(math::aabb<3>(math::vec<3>(-1, -5, -1), math::vec<3>(0, -4, 0))) == math::plane<>::POSITIVE);
	BOOST_REQUIRE(pl.classify(math::aabb<3>(math::vec<3>(-2, -2, -2), math::vec<3>(0, 0, 0))) == math::plane<>::INTERSECTS);
	BOOST_REQUIRE(pl.classify(math::aabb<3>(math::vec<3>(-1, -1, -1), math::vec<3>(1, 1, 1))) == math::plane<>::INTERSECTS);
}

BOOST_AUTO_TEST_CASE(classify_aabb_4)
{
	math::plane<> pl(math::vec<3>(0, 2, 0), math::vec<3>(1, 1, 1));

	BOOST_REQUIRE(pl.classify(math::aabb<3>(math::vec<3>(-1, -1, -1), math::vec<3>(0, 0, 0))) == math::plane<>::NEGATIVE);
}

BOOST_AUTO_TEST_CASE(classify_obb_1)
{
	math::plane<> pl(math::vec<3>(0, 0, 0), math::vec<3>(1, 1, 1));

	BOOST_REQUIRE(pl.classify(
			math::obb<3>(math::vec<3>(0, 1, 0), math::vec<3>(1, 0, 0), math::vec<3>(0, 1, 0), math::vec<3>(0, 0, 1)))
		== math::plane<>::POSITIVE);

	BOOST_REQUIRE(pl.classify(
			math::obb<3>(math::vec<3>(-1, -2, -1), math::vec<3>(1, 0, 0), math::vec<3>(0, 1, 0), math::vec<3>(0, 0, 1)))
		== math::plane<>::NEGATIVE);
}

BOOST_AUTO_TEST_CASE(classify_obb_2)
{
	math::plane<> pl(math::vec<3>(0, 0, 0), math::vec<3>(1, 1, 1));

	BOOST_REQUIRE(pl.classify(
			math::obb<3>(
				math::vec<3>(-1, -1, -1),
				math::vec<3>(11, 10, 10),
				math::vec<3>(10, 11, 10),
				math::vec<3>(10, 10, 11)))
		== math::plane<>::INTERSECTS);
}

BOOST_AUTO_TEST_CASE(raytrace_1)
{
	math::plane<> pl(math::vec<3>(0, 0, 0), math::vec<3>(0, 1, 0));

	math::ray<3> r(math::vec<3>(0, 1, 0), math::vec<3>(0, -10, 0));

	math::scalar t;
	math::vec<3> v;

	pl.trace(r, t, v);

	BOOST_REQUIRE((v - math::vec<3>(0, 0, 0)).length() < math::EPSILON);
}

BOOST_AUTO_TEST_CASE(raytrace_2)
{
	math::plane<> pl(math::vec<3>(0, 0, 0), math::vec<3>(0, 1, 0));

	math::ray<3> r(math::vec<3>(7, 1, 11), math::vec<3>(0, -10, 0));

	math::scalar t;
	math::vec<3> v;

	pl.trace(r, t, v);

	BOOST_REQUIRE((v - math::vec<3>(7, 0, 11)).length() < math::EPSILON);
}

BOOST_AUTO_TEST_CASE(raytrace_3)
{
	math::plane<> pl(math::vec<3>(1, 3, 5), math::vec<3>(0, 1, 0));

	math::ray<3> r(math::vec<3>(7 + 1, 1 + 3, 11 + 5), math::vec<3>(0, -10, 0));

	math::scalar t;
	math::vec<3> v;

	pl.trace(r, t, v);

	BOOST_REQUIRE((v - math::vec<3>(7 + 1, 0 + 3, 11 + 5)).length() < math::EPSILON);
}

BOOST_AUTO_TEST_CASE(raytrace_4)
{
	math::plane<> pl(math::vec<3>(1, 3, 5), math::vec<3>(1, 1, 1));

	math::ray<3> r(math::vec<3>(1, 10, 5), math::vec<3>(0, -10, 0));

	math::scalar t;
	math::vec<3> v;

	pl.trace(r, t, v);

	BOOST_REQUIRE((v - math::vec<3>(1, 3, 5)).length() < math::EPSILON);
}

BOOST_AUTO_TEST_SUITE_END()

