
#include <boost/test/unit_test.hpp>
#include "camera.h"

void camera_views_are_equal(graphic::Camera &c1, graphic::Camera &c2)
{
	BOOST_ASSERT ((c1.get_eye_point() - c2.get_eye_point()).length_sq() < math::EPSILON);
	BOOST_ASSERT ((c1.get_up_dir() - c2.get_up_dir()).length_sq() < math::EPSILON);
	BOOST_ASSERT ((c1.get_view_dir() - c2.get_view_dir()).length_sq() < math::EPSILON);
	BOOST_ASSERT ((c1.get_cross_dir() - c2.get_cross_dir()).length_sq() < math::EPSILON);
	BOOST_ASSERT (c1.get_view_matrix() == c2.get_view_matrix());
}

BOOST_AUTO_TEST_SUITE (test_camera)

BOOST_AUTO_TEST_CASE (test_translation_rotation_1)
{
	graphic::Camera c;
	c.set_translation_rotation(math::vec<3>(0, 0, 0), math::vec<3>(0, 0, 0));

	BOOST_ASSERT (((math::vec<3>(0, 0, 0) * c.get_view_matrix()) - math::vec<3>(0, 0, 0)).length_sq() < math::EPSILON);
	BOOST_ASSERT ((math::vec<3>(0, 0, 0) - c.get_eye_point()).length_sq() < math::EPSILON);
	BOOST_ASSERT ((math::vec<3>(0, 0, 1) - c.get_view_dir()).length_sq() < math::EPSILON);
	BOOST_ASSERT ((math::vec<3>(0, 1, 0) - c.get_up_dir()).length_sq() < math::EPSILON);
	BOOST_ASSERT ((math::vec<3>(1, 0, 0) - c.get_cross_dir()).length_sq() < math::EPSILON);
}

BOOST_AUTO_TEST_CASE (test_translation_rotation_2)
{
	graphic::Camera c;
	c.set_translation_rotation(math::vec<3>(-123, -456, -789), math::vec<3>(0, 0, 0));

	BOOST_ASSERT (((math::vec<3>(0, 0, 0) * c.get_view_matrix()) + math::vec<3>(123, 456, 789)).length_sq() < math::EPSILON);
	BOOST_ASSERT ((math::vec<3>(123, 456, 789) - c.get_eye_point()).length_sq() < math::EPSILON);
	BOOST_ASSERT ((math::vec<3>(0, 0, 1) - c.get_view_dir()).length_sq() < math::EPSILON);
	BOOST_ASSERT ((math::vec<3>(0, 1, 0) - c.get_up_dir()).length_sq() < math::EPSILON);
	BOOST_ASSERT ((math::vec<3>(1, 0, 0) - c.get_cross_dir()).length_sq() < math::EPSILON);
}

BOOST_AUTO_TEST_CASE (test_translation_rotation_3)
{
	graphic::Camera c;
	c.set_translation_rotation(math::vec<3>(0, 0, 0), math::vec<3>(-math::PI/4, 0, 0));

	BOOST_ASSERT (((math::vec<3>(0, 0, 0) * c.get_view_matrix()) + math::vec<3>(0, 0, 0)).length_sq() < math::EPSILON);
	BOOST_ASSERT ((math::vec<3>(0, 0, 0) - c.get_eye_point()).length_sq() < math::EPSILON);
	BOOST_ASSERT ((math::vec<3>(0, 0.707106f, 0.707106f) - c.get_view_dir()).length_sq() < math::EPSILON);
	BOOST_ASSERT ((math::vec<3>(0, 0.707106f, -0.707106f) - c.get_up_dir()).length_sq() < math::EPSILON);
	BOOST_ASSERT ((math::vec<3>(1, 0, 0) - c.get_cross_dir()).length_sq() < math::EPSILON);
}

BOOST_AUTO_TEST_CASE (test_translation_rotation_4)
{
	graphic::Camera c;
	c.set_translation_rotation(math::vec<3>(0, 0, 0), math::vec<3>(math::PI/2, 0, 0));

	BOOST_ASSERT (((math::vec<3>(0, 0, 0) * c.get_view_matrix()) + math::vec<3>(0, 0, 0)).length_sq() < math::EPSILON);
	BOOST_ASSERT ((math::vec<3>(0, 0, 0) - c.get_eye_point()).length_sq() < math::EPSILON);
	BOOST_ASSERT ((math::vec<3>(0, -1, 0) - c.get_view_dir()).length_sq() < math::EPSILON);
	BOOST_ASSERT ((math::vec<3>(0, 0, 1) - c.get_up_dir()).length_sq() < math::EPSILON);
	BOOST_ASSERT ((math::vec<3>(1, 0, 0) - c.get_cross_dir()).length_sq() < math::EPSILON);
}

BOOST_AUTO_TEST_CASE (test_eye_lookat_up_1)
{
	graphic::Camera c;
	c.set_eye_lookat_up(math::vec<3>(0, 0, 0), math::vec<3>(0, 0, 1), math::vec<3>(0, 1, 0));

	BOOST_ASSERT (((math::vec<3>(0, 0, 0) * c.get_view_matrix()) - math::vec<3>(0, 0, 0)).length_sq() < math::EPSILON);
	BOOST_ASSERT ((math::vec<3>(0, 0, 0) - c.get_eye_point()).length_sq() < math::EPSILON);
	BOOST_ASSERT ((math::vec<3>(0, 0, 1) - c.get_view_dir()).length_sq() < math::EPSILON);
	BOOST_ASSERT ((math::vec<3>(0, 1, 0) - c.get_up_dir()).length_sq() < math::EPSILON);
	BOOST_ASSERT ((math::vec<3>(1, 0, 0) - c.get_cross_dir()).length_sq() < math::EPSILON);
}

BOOST_AUTO_TEST_CASE (test_eye_lookat_up_2)
{
	graphic::Camera c;
	c.set_eye_lookat_up(math::vec<3>(123, 456, 789), math::vec<3>(123, 456, 790), math::vec<3>(0, 1, 0));

	BOOST_ASSERT (((math::vec<3>(0, 0, 0) * c.get_view_matrix()) + math::vec<3>(123, 456, 789)).length_sq() < math::EPSILON);
	BOOST_ASSERT ((math::vec<3>(123, 456, 789) - c.get_eye_point()).length_sq() < math::EPSILON);
	BOOST_ASSERT ((math::vec<3>(0, 0, 1) - c.get_view_dir()).length_sq() < math::EPSILON);
	BOOST_ASSERT ((math::vec<3>(0, 1, 0) - c.get_up_dir()).length_sq() < math::EPSILON);
	BOOST_ASSERT ((math::vec<3>(1, 0, 0) - c.get_cross_dir()).length_sq() < math::EPSILON);
}

BOOST_AUTO_TEST_CASE (test_equal_1)
{
	graphic::Camera c1, c2;
	c1.set_eye_lookat_up(math::vec<3>(0, 0, 0), math::vec<3>(0, 0, 1), math::vec<3>(0, 1, 0));
	c2.set_translation_rotation(math::vec<3>(0, 0, 0), math::vec<3>(0, 0, 0));
	camera_views_are_equal(c1, c2);
}

BOOST_AUTO_TEST_CASE (test_equal_2)
{
	graphic::Camera c1, c2;
	c1.set_eye_lookat_up(math::vec<3>(1, 0, 0), math::vec<3>(1, 0, 1), math::vec<3>(0, 1, 0));
	c2.set_translation_rotation(math::vec<3>(-1, 0, 0), math::vec<3>(0, 0, 0));
	camera_views_are_equal(c1, c2);
}

BOOST_AUTO_TEST_CASE (test_equal_3)
{
	graphic::Camera c1, c2;
	c1.set_eye_lookat_up(math::vec<3>(0, 1, 0), math::vec<3>(0, 0, 0), math::vec<3>(1, 0, 0));
	c2.set_translation_rotation(math::vec<3>(0, -1, 0), math::vec<3>(0, 0, -math::PI/2));
	camera_views_are_equal(c1, c2);
}

BOOST_AUTO_TEST_CASE (test_equal_4)
{
	graphic::Camera c1, c2;
	c1.set_eye_lookat_up(math::vec<3>(0, 0, 0), math::vec<3>(1, -1, 0), math::vec<3>(1, 1, 0));
	c2.set_translation_rotation(math::vec<3>(0, 0, 0), math::vec<3>(0, 0, -math::PI / 4));
	camera_views_are_equal(c1, c2);
}

BOOST_AUTO_TEST_CASE (test_equal_5)
{
	graphic::Camera c1, c2;
	c1.set_eye_lookat_up(math::vec<3>(0, 0, 0), math::vec<3>(1, 1, 0), math::vec<3>(-1, 1, 0));
	c2.set_translation_rotation(math::vec<3>(0, 0, 0), math::vec<3>(0, 0, math::PI / 4));
	camera_views_are_equal(c1, c2);
}

BOOST_AUTO_TEST_SUITE_END()
