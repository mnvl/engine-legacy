
#include <cppunit/extensions/HelperMacros.h>
#include <math/intersection.h>

class test_intersections: public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE(test_intersections);
	CPPUNIT_TEST(ray_ray_intersection);
	CPPUNIT_TEST(test_sphere_line_intersection_1);
	CPPUNIT_TEST_SUITE_END();

	void ray_ray_intersection()
	{
		math::ray<2> r1, r2;
		math::scalar s;

		r1.origin.set(0, 0);
		r1.direction.set(100, 500);

		r2.origin.set(1, 0);
		r2.direction.set(-1, 3);

		math::intersect(r1, r2, s);

		math::vec<2> p1 = r1.origin + r1.direction * s;
		CPPUNIT_ASSERT (math::intersect(r2, p1));

		CPPUNIT_ASSERT (math::abs(s - 0.00375f) < math::EPSILON);
	}

	void test_sphere_line_intersection_1()
	{
		math::sphere<2> s;
		s.centre.set(1, 0);
		s.radius = 1;

		math::line<2> l;
		l.A.set(0, 1);
		l.B.set(0, -1);

		math::vec<2> p1, p2;
		int npoints = math::intersect(l, s, p1, p2);

		CPPUNIT_ASSERT (npoints == 1);
		CPPUNIT_ASSERT ((p1 - math::vec<2>(0, 0)).length() < math::EPSILON);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(test_intersections); 