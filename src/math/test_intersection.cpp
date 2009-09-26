
#include <boost/test/unit_test.hpp>
#include <math/intersection.h>

BOOST_AUTO_TEST_SUITE (test_intersection)

BOOST_AUTO_TEST_CASE (ray_ray_intersection)
{
	math::ray<2> r1, r2;
	math::scalar s;

	r1.origin.set(0, 0);
	r1.direction.set(100, 500);

	r2.origin.set(1, 0);
	r2.direction.set(-1, 3);

	math::intersect(r1, r2, s);

	math::vec<2> p1 = r1.origin + r1.direction * s;
	BOOST_ASSERT (math::intersect(r2, p1));

	BOOST_ASSERT (math::abs(s - 0.00375f) < math::EPSILON);
}

BOOST_AUTO_TEST_CASE (test_sphere_line_intersection_1)
{
	math::sphere<2> s;
	s.centre.set(1, 0);
	s.radius = 1;

	math::line<2> l;
	l.A.set(0, 1);
	l.B.set(0, -1);

	math::vec<2> p1, p2;
	int npoints = math::intersect(l, s, p1, p2);

	BOOST_ASSERT (npoints == 1);
	BOOST_ASSERT ((p1 - math::vec<2>(0, 0)).length() < math::EPSILON);
}

BOOST_AUTO_TEST_SUITE_END ()
