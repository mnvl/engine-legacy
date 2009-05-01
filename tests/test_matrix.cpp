
#include <math/matrix.h>

#include <graphic/collada_loader.h>
#include <cppunit/extensions/HelperMacros.h>

class test_matrix: public CppUnit::TestFixture {
public:
	CPPUNIT_TEST_SUITE(test_matrix);
	CPPUNIT_TEST(test_axis_angle);
	CPPUNIT_TEST_SUITE_END();

	void test_axis_angle()
	{
		math::matrix<4,4> m;
		m.rotation(math::vec<3>(0,2,0), math::PI / 2);

		math::vec<3> v2 = math::vec<3>(1,0,0) * m;

		CPPUNIT_ASSERT ((v2.length() - 2.0f) < math::EPSILON);
		CPPUNIT_ASSERT ((v2 - math::vec<3>(0, 0, -1.0f)).length() < math::EPSILON);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(test_matrix);