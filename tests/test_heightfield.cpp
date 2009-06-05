
#include <boost/cstdint.hpp>
#include <cppunit/extensions/HelperMacros.h>
#include <math/heightfield.h>

class test_heightfield: public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(test_heightfield);
    CPPUNIT_TEST(test_y_at);
	CPPUNIT_TEST_SUITE_END();

    void test_y_at()
    {
        static boost::uint8_t const heights[] = {
            5,  10,  11,
            1,   4,  15,
            10,  8,  17,
            16, 12,  30
        };

        math::matrix<4,4> tf;
        tf.identity();

        math::heightfield<boost::uint8_t> hf(tf, 3, 3, math::vec<3>(3, 1, 3));
        hf.load_from_buffer(heights);

        CPPUNIT_ASSERT(abs(hf.y_at(math::vec<3>( .00f, 0,  .00f)) -  5.00f) < math::EPSILON);
        CPPUNIT_ASSERT(abs(hf.y_at(math::vec<3>( .50f, 0,  .00f)) -  3.00f) < math::EPSILON);
        CPPUNIT_ASSERT(abs(hf.y_at(math::vec<3>( .00f, 0,  .50f)) -  7.50f) < math::EPSILON);
        CPPUNIT_ASSERT(abs(hf.y_at(math::vec<3>( .50f, 0,  .50f)) -  5.50f) < math::EPSILON);
		CPPUNIT_ASSERT(abs(hf.y_at(math::vec<3>(1.00f, 0, 0.00f)) - 10.00f) < math::EPSILON);
		CPPUNIT_ASSERT(abs(hf.y_at(math::vec<3>(0.00f, 0, 1.00f)) -  1.00f) < math::EPSILON);
		CPPUNIT_ASSERT(abs(hf.y_at(math::vec<3>(1.00f, 0, 1.00f)) -  4.00f) < math::EPSILON);
		CPPUNIT_ASSERT(abs(hf.y_at(math::vec<3>(0.75f, 0, 0.75f)) -  5.25f) < math::EPSILON);
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(test_heightfield);
