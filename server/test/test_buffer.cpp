
#define BOOST_TEST_MODULE test_buffer
#include <boost/test/included/unit_test.hpp>
#include <server/buffer.h>

BOOST_AUTO_TEST_SUITE(test_buffer)

BOOST_AUTO_TEST_CASE (test_buffer_1)
{
	server::buffer a, b;
	b = a;
}

BOOST_AUTO_TEST_CASE (test_buffer_2)
{
	server::buffer a;
	a.enlarge();
	BOOST_CHECK_THROW(a.enlarge(), std::bad_alloc);
}

BOOST_AUTO_TEST_SUITE_END()
