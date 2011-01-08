//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2011 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include "bresenham_supercover.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (brezenham_supercover)

BOOST_AUTO_TEST_CASE (one_point)
{
	math::bresenham_supercover<> bs(math::vec<2, int>(1, 5), math::vec<2, int>(1, 5));

	math::vec<2, int> position;
	bool flag;

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 1);
	BOOST_REQUIRE (position.y == 5);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (flag == true);
}

BOOST_AUTO_TEST_CASE (two_points_digonal_1)
{
	math::bresenham_supercover<> bs(math::vec<2, int>(1, 5), math::vec<2, int>(2, 6));

	math::vec<2, int> position;
	bool flag;

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 1);
	BOOST_REQUIRE (position.y == 5);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 1);
	BOOST_REQUIRE (position.y == 6);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 2);
	BOOST_REQUIRE (position.y == 5);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 2);
	BOOST_REQUIRE (position.y == 6);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (flag == true);
}

BOOST_AUTO_TEST_CASE (two_points_digonal_2)
{
	math::bresenham_supercover<> bs(math::vec<2, int>(1, 5), math::vec<2, int>(2, 4));

	math::vec<2, int> position;
	bool flag;

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 1);
	BOOST_REQUIRE (position.y == 5);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 1);
	BOOST_REQUIRE (position.y == 4);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 2);
	BOOST_REQUIRE (position.y == 5);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 2);
	BOOST_REQUIRE (position.y == 4);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (flag == true);
}


BOOST_AUTO_TEST_CASE (two_points_digonal_3)
{
	math::bresenham_supercover<> bs(math::vec<2, int>(2, 6), math::vec<2, int>(1, 5));

	math::vec<2, int> position;
	bool flag;

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 2);
	BOOST_REQUIRE (position.y == 6);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 2);
	BOOST_REQUIRE (position.y == 5);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 1);
	BOOST_REQUIRE (position.y == 6);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 1);
	BOOST_REQUIRE (position.y == 5);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (flag == true);
}

BOOST_AUTO_TEST_CASE (two_points_digonal_4)
{
	math::bresenham_supercover<> bs(math::vec<2, int>(2, 4), math::vec<2, int>(1, 5));

	math::vec<2, int> position;
	bool flag;

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 2);
	BOOST_REQUIRE (position.y == 4);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 2);
	BOOST_REQUIRE (position.y == 5);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 1);
	BOOST_REQUIRE (position.y == 4);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 1);
	BOOST_REQUIRE (position.y == 5);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (flag == true);
}

BOOST_AUTO_TEST_CASE (three_points_digonal_1)
{
	math::bresenham_supercover<> bs(math::vec<2, int>(1, 5), math::vec<2, int>(3, 6));

	math::vec<2, int> position;
	bool flag;

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 1);
	BOOST_REQUIRE (position.y == 5);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 2);
	BOOST_REQUIRE (position.y == 5);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 2);
	BOOST_REQUIRE (position.y == 6);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 3);
	BOOST_REQUIRE (position.y == 6);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (flag == true);
}

BOOST_AUTO_TEST_CASE (three_points_digonal_2)
{
	math::bresenham_supercover<> bs(math::vec<2, int>(1, 5), math::vec<2, int>(3, 4));

	math::vec<2, int> position;
	bool flag;

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 1);
	BOOST_REQUIRE (position.y == 5);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 2);
	BOOST_REQUIRE (position.y == 5);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 2);
	BOOST_REQUIRE (position.y == 4);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 3);
	BOOST_REQUIRE (position.y == 4);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (flag == true);
}

BOOST_AUTO_TEST_CASE (three_points_digonal_3)
{
	math::bresenham_supercover<> bs(math::vec<2, int>(3, 6), math::vec<2, int>(1, 5));

	math::vec<2, int> position;
	bool flag;

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 3);
	BOOST_REQUIRE (position.y == 6);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 2);
	BOOST_REQUIRE (position.y == 6);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 2);
	BOOST_REQUIRE (position.y == 5);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 1);
	BOOST_REQUIRE (position.y == 5);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (flag == true);
}

BOOST_AUTO_TEST_CASE (three_points_digonal_4)
{
	math::bresenham_supercover<> bs(math::vec<2, int>(3, 4), math::vec<2, int>(1, 5));

	math::vec<2, int> position;
	bool flag;

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 3);
	BOOST_REQUIRE (position.y == 4);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 2);
	BOOST_REQUIRE (position.y == 4);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 2);
	BOOST_REQUIRE (position.y == 5);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 1);
	BOOST_REQUIRE (position.y == 5);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (flag == true);
}

BOOST_AUTO_TEST_CASE (many_points)
{
	math::bresenham_supercover<> bs(math::vec<2, int>(1, 1), math::vec<2, int>(12, 9));

	math::vec<2, int> position;
	bool flag;

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 1);
	BOOST_REQUIRE (position.y == 1);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 2);
	BOOST_REQUIRE (position.y == 1);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 2);
	BOOST_REQUIRE (position.y == 2);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 3);
	BOOST_REQUIRE (position.y == 2);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 3);
	BOOST_REQUIRE (position.y == 3);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 4);
	BOOST_REQUIRE (position.y == 3);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 4);
	BOOST_REQUIRE (position.y == 4);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 5);
	BOOST_REQUIRE (position.y == 4);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 6);
	BOOST_REQUIRE (position.y == 4);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 6);
	BOOST_REQUIRE (position.y == 5);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 7);
	BOOST_REQUIRE (position.y == 5);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 7);
	BOOST_REQUIRE (position.y == 6);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 8);
	BOOST_REQUIRE (position.y == 6);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 9);
	BOOST_REQUIRE (position.y == 6);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 9);
	BOOST_REQUIRE (position.y == 7);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 10);
	BOOST_REQUIRE (position.y == 7);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 10);
	BOOST_REQUIRE (position.y == 8);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 11);
	BOOST_REQUIRE (position.y == 8);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 11);
	BOOST_REQUIRE (position.y == 9);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (position.x == 12);
	BOOST_REQUIRE (position.y == 9);
	BOOST_REQUIRE (flag == false);

	boost::tie(position, flag) = bs.get();
	BOOST_REQUIRE (flag == true);
}

BOOST_AUTO_TEST_SUITE_END()


