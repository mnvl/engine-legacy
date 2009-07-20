
#define BOOST_TEST_MODULE test_table
#include <boost/test/included/unit_test.hpp>
#include <script/table.h>

BOOST_AUTO_TEST_SUITE(test_triangle)

BOOST_AUTO_TEST_CASE (test_table_get_set)
{
	script::table t;

	t.resize_fields(2);

	t.set<double>(0, 12);
	BOOST_ASSERT(t.get<double>(0) == 12);

	t.set<std::string>(1, "bebe");
	BOOST_ASSERT(t.get<std::string>(1) == "bebe");
}

BOOST_AUTO_TEST_SUITE_END()
