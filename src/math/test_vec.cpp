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
#include "vec.h"

BOOST_AUTO_TEST_SUITE (vec)

BOOST_AUTO_TEST_CASE (perpendiculars)
{
	for (int i = 0; i < 1000; i++)
	{
		math::vec<3> v(math::scalar(rand() * 2) / RAND_MAX - 1, math::scalar(rand() * 2) / RAND_MAX - 1,
			math::scalar(rand() * 2) / RAND_MAX - 1);

		math::vec<3> xy = v.perpendicular_xy();
		math::vec<3> yz = v.perpendicular_yz();
		math::vec<3> xz = v.perpendicular_xz();

		BOOST_CHECK (math::abs(xy & v) < math::EPSILON);
		BOOST_CHECK (math::abs(yz & v) < math::EPSILON);
		BOOST_CHECK (math::abs(xz & v) < math::EPSILON);
	}
}

BOOST_AUTO_TEST_SUITE_END()
