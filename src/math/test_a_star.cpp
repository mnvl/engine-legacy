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
#include "a_star.h"

BOOST_AUTO_TEST_SUITE(a_star)

BOOST_AUTO_TEST_CASE(position_and_node)
{
	class my_scorer: public math::a_star::scorer {
	public:
		virtual math::scalar get_score(math::vec<3> const &position, math::vec<3> const &goal) const {
			return 1;
		}
	};

	my_scorer scorer;
	math::a_star as(math::vec<2>(-10, 20), math::vec<2>(20, 10), 0.1f, &scorer);

	BOOST_REQUIRE((as.node_to_position(as.position_to_node(math::vec<2>(0, 0))) - math::vec<2>(0, 0)).length_sq() < 1e-6);
	BOOST_REQUIRE((as.node_to_position(as.position_to_node(math::vec<2>(11, 22))) - math::vec<2>(11, 22)).length_sq() < 1e-6);
	BOOST_REQUIRE((as.node_to_position(as.position_to_node(math::vec<2>(-401, 62))) - math::vec<2>(-401, 62)).length_sq() < 1e-6);
}

BOOST_AUTO_TEST_CASE(simple_path)
{
	class my_scorer: public math::a_star::scorer {
	public:
		virtual math::scalar get_score(math::vec<3> const &position, math::vec<3> const &goal) const {
			return (goal - position).length();
		}
	};

	my_scorer scorer;
	math::a_star as(math::vec<2>(-10, 20), math::vec<2>(20, 10), 0.1f, &scorer);

	as.calculate();
}

BOOST_AUTO_TEST_SUITE_END()
