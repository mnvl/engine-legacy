
#define BOOST_TEST_MODULE test_table
#include <boost/test/included/unit_test.hpp>
#include <script/table.h>
#include <script/descr.h>

BOOST_AUTO_TEST_SUITE(test_triangle)

BOOST_AUTO_TEST_CASE (test_table_get_set)
{
	script::table t(0);

	t.resize_fields(2);

	t.set<double>(0, 12);
	BOOST_ASSERT(t.get<double>(0) == 12);

	t.set<std::string>(1, "bebe");
	BOOST_ASSERT(t.get<std::string>(1) == "bebe");
}

BOOST_AUTO_TEST_CASE (test_descr_construct)
{
	script::descr d("test");

	BOOST_ASSERT (d.add_field("a") == 0);
	BOOST_ASSERT (d.add_field("b") == 1);
	BOOST_ASSERT (d.add_field("c") == 2);
	BOOST_ASSERT (d.add_field("b") == 1);
	BOOST_ASSERT (d.add_field("c") == 2);
}

BOOST_AUTO_TEST_CASE (test_table_descr_get_set)
{
	script::descr d("test");
	d.add_field("a");
	d.add_field("b");

	script::table t(&d);

	t.set<double>("a", 12);
	BOOST_ASSERT (t.get<double>("a") == 12);

	t.set<std::string>("b", "x2");
	BOOST_ASSERT (t.get<std::string>("b") == "x2");
}

BOOST_AUTO_TEST_SUITE_END()
