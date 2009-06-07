
#include <cppunit/extensions/HelperMacros.h>
#include <math/triangle.h>

class test_triangle: public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(test_triangle);
	CPPUNIT_TEST(test_trace);
	CPPUNIT_TEST_SUITE_END();

	void test_trace()
	{
		math::triangle<3> tri(math::vec<3>(0, 1, 0), math::vec<3>(0, 3, 1), math::vec<3>(1, 3, 0));
		math::scalar t;
		math::vec<3> v;
		bool result = tri.trace(math::ray<3>(math::vec<3>(-1, 2, 0), math::vec<3>(10, 0, 0)), t, v);
		CPPUNIT_ASSERT (result == true);
		CPPUNIT_ASSERT ((math::vec<3>(0.5f, 2, 0) - v).length_sq() < math::EPSILON);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(test_triangle); 