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
#include "obb.h"

BOOST_AUTO_TEST_SUITE (obb)

BOOST_AUTO_TEST_CASE (forward_point)
{
	math::obb<3> bounds;
	bounds.origin.set(0, 0, 0);
	bounds.tangent.set(2, 0, 0);
	bounds.normal.set(0, 2, 0);
	bounds.binormal.set(0, 0, 2);

	math::vec<3> point(10, 0, 0), point_local;
	bounds.forward_point(point_local, point);

	BOOST_ASSERT(math::abs(point_local.x - 5) < math::EPSILON);
	BOOST_ASSERT(math::abs(point_local.y - 0) < math::EPSILON);
	BOOST_ASSERT(math::abs(point_local.z - 0) < math::EPSILON);
}

BOOST_AUTO_TEST_SUITE_END()

