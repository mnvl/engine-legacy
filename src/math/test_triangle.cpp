//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2012 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <boost/test/unit_test.hpp>
#include "triangle.h"

BOOST_AUTO_TEST_SUITE(test_triangle)

BOOST_AUTO_TEST_CASE(test_triangle_trace)
{
	math::triangle<3> tri(math::vec<3>(0, 1, 0), math::vec<3>(0, 3, 1), math::vec<3>(1, 3, 0));
	math::scalar t;
	math::vec<3> v;
	bool result = tri.trace(math::ray<3>(math::vec<3>(-1, 2, 0), math::vec<3>(10, 0, 0)), t, v);
	BOOST_CHECK (result == true);
	BOOST_CHECK ((math::vec<3>(0.5f, 2, 0) - v).length() < math::EPSILON);
}

BOOST_AUTO_TEST_SUITE_END()
