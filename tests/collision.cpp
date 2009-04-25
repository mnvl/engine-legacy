
#include <cppunit/extensions/HelperMacros.h>
#include <math/collision.h>

class test_collisions: public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE(test_collisions);
	CPPUNIT_TEST(test_sphere_line_collision_1);
	CPPUNIT_TEST(test_sphere_line_collision_2);
	CPPUNIT_TEST(test_sphere_line_collision_3);
	CPPUNIT_TEST(test_sphere_obb_collision_1);
	CPPUNIT_TEST(test_sphere_obb_collision_2);
	CPPUNIT_TEST_SUITE_END();

	void test_sphere_line_collision_1() {
		math::sphere<2> sph;
		sph.centre.set(10, 10);
		sph.radius = 1;

		math::line<2> l;
		l.A.set(-100, 0);
		l.B.set(100, 0);

		math::collision_info<2> ci;
		math::collide(l, sph, math::vec<2>(-2, -1), ci);

		assert(ci.happened == true);
		assert(math::abs(ci.time - 9.0f) < math::EPSILON);
	}

	void test_sphere_line_collision_2() {
		math::sphere<2> sph;
		sph.centre.set(10, 0.5f);
		sph.radius = 1;

		math::line<2> l;
		l.A.set(0, 0);
		l.B.set(0, 1);

		math::collision_info<2> ci;
		math::collide(l, sph, math::vec<2>(-1, 0), ci);

		assert(ci.happened == true);
		assert(math::abs(ci.time - 9.0f) < math::EPSILON);
		assert((ci.position - math::vec<2>(0, 0.5f)).length_sq() < math::EPSILON);
		assert((ci.normal - math::vec<2>(1, 0)).length_sq() < math::EPSILON);
	}

	void test_sphere_line_collision_3() {
		math::line<2> l;
		l.A.set(1, -10);
		l.B.set(1, 10);

		math::sphere<2> s;
		s.centre.set(10, 10);
		s.radius = 1;

		math::collision_info<2> ci;
		math::collide(l, s, math::vec<2>(-1, -1), ci);

		assert ((ci.position - math::vec<2>(1, 2)).length() < math::EPSILON);
		assert ((ci.time - 8) < math::EPSILON);
	}

	void test_sphere_obb_collision_1() {
		math::obb<2> obb;
		obb.origin.set(0, 0);
		obb.tangent.set(1, 0);
		obb.normal.set(0, 1);

		math::sphere<2> sph;
		sph.centre.set(0.5, 10);
		sph.radius = 1;

		math::collision_info<2> ci;
		math::collide(obb, sph, math::vec<2>(0, -1), ci);

		assert ((ci.position - math::vec<2>(0.5, 1)).length() < math::EPSILON);
		assert ((ci.normal - math::vec<2>(0, 1)).length() < math::EPSILON);
		assert (math::abs(ci.time - 8) < math::EPSILON);
	}

	void test_sphere_obb_collision_2() {
		math::obb<2> obb;
		obb.origin.set(0, 0);
		obb.tangent.set(1, 0);
		obb.normal.set(0, 1);

		math::sphere<2> sph;
		sph.centre.set(10, 0.5f);
		sph.radius = 1;

		math::collision_info<2> ci;

		math::collide(obb, sph, math::vec<2>(-1.0f, -0.01f), ci);

		assert ((ci.position - math::vec<2>(1, 0.42f)).length() < math::EPSILON);
		assert ((ci.normal - math::vec<2>(1, 0)).length() < math::EPSILON);
		assert ((ci.time - 8) < math::EPSILON);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(test_collisions); 