
#include <boost/cstdint.hpp>
#include <boost/scoped_ptr.hpp>
#include <cppunit/extensions/HelperMacros.h>
#include <math/heightfield.h>

class test_heightfield: public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(test_heightfield);
    CPPUNIT_TEST(test_y_at);
    CPPUNIT_TEST(test_raytrace);
	CPPUNIT_TEST_SUITE_END();
private:
    typedef math::heightfield<boost::uint8_t> hf_t;
    boost::scoped_ptr<hf_t> hf_;

public:
    void setUp()
    {
        math::matrix<4,4> tf;
        tf.identity();

        hf_.reset(new hf_t(tf, 3, 3, math::vec<3>(3, 1, 3)));

        static boost::uint8_t const heights[] = {
             5, 10, 11,
             1,  4,  5,
            10,  5,  5
        };

        hf_->load_from_buffer(heights);
    }

    void tearDown()
    {
        hf_.reset(0);
    }

    void test_y_at()
    {
        CPPUNIT_ASSERT(abs(hf_->y_at(math::vec<3>( .00f, 0,  .00f)) -  5.00f) < math::EPSILON);
        CPPUNIT_ASSERT(abs(hf_->y_at(math::vec<3>( .50f, 0,  .00f)) -  3.00f) < math::EPSILON);
        CPPUNIT_ASSERT(abs(hf_->y_at(math::vec<3>( .00f, 0,  .50f)) -  7.50f) < math::EPSILON);
        CPPUNIT_ASSERT(abs(hf_->y_at(math::vec<3>( .50f, 0,  .50f)) -  5.50f) < math::EPSILON);
		CPPUNIT_ASSERT(abs(hf_->y_at(math::vec<3>(1.00f, 0, 0.00f)) - 10.00f) < math::EPSILON);
		CPPUNIT_ASSERT(abs(hf_->y_at(math::vec<3>(0.00f, 0, 1.00f)) -  1.00f) < math::EPSILON);
		CPPUNIT_ASSERT(abs(hf_->y_at(math::vec<3>(1.00f, 0, 1.00f)) -  4.00f) < math::EPSILON);
		CPPUNIT_ASSERT(abs(hf_->y_at(math::vec<3>(0.75f, 0, 0.75f)) -  5.25f) < math::EPSILON);
    }

    void test_raytrace()
    {
		{
			math::contact_info<3> ci = hf_->trace(math::ray<3>(math::vec<3>(1.5, 10, 1.5), math::vec<3>(1, -100, 1)));
			CPPUNIT_ASSERT (ci.happened == true);
			CPPUNIT_ASSERT (ci.penetrated == false);
			CPPUNIT_ASSERT ((ci.position - math::vec<3>(1.55f, 5.1f, 1.55f)).length_sq() < 0.01f);
			CPPUNIT_ASSERT ((ci.normal - math::vec<3>(-0.57f, 0.57f, -0.57f)).length_sq() < 0.01f);
		}

		{
			math::contact_info<3> ci = hf_->trace(math::ray<3>(math::vec<3>(-1, 5, 0.5f), math::vec<3>(1, 0, 0)));
			CPPUNIT_ASSERT (ci.happened == true);
			CPPUNIT_ASSERT (ci.penetrated == false);
			CPPUNIT_ASSERT ((ci.position - math::vec<3>(0.62f, 5.0f, 0.5f)).length_sq() < 0.01f);
			CPPUNIT_ASSERT ((ci.normal - math::vec<3>(0.62f, 0.15f, -0.78f)).length_sq() < 0.01f);
		}
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(test_heightfield);
