
#include <boost/test/unit_test.hpp>
#include "camera.h"

void camera_views_are_equal(graphic::Camera &c1, graphic::Camera &c2)
{
	BOOST_ASSERT ((c1.get_eye_point() - c2.get_eye_point()).length_sq() < math::EPSILON);
	BOOST_ASSERT ((c1.get_up_dir() - c2.get_up_dir()).length_sq() < math::EPSILON);
	BOOST_ASSERT ((c1.get_view_dir() - c2.get_view_dir()).length_sq() < math::EPSILON);
	BOOST_ASSERT ((c1.get_cross_dir() - c2.get_cross_dir()).length_sq() < math::EPSILON);
}

BOOST_AUTO_TEST_SUITE (test_camera)

BOOST_AUTO_TEST_CASE (test_view_1)
{
	graphic::Camera c1, c2;
	c1.set_eye_lookat_up(math::vec<3>(0, 0, 0), math::vec<3>(1, 0, 0), math::vec<3>(0, 1, 0));
	c2.set_translation_rotation(math::vec<3>(0, 0, 0), math::vec<3>(0, 0, 0));
	camera_views_are_equal(c1, c2);
}

BOOST_AUTO_TEST_CASE (test_view_2)
{
	graphic::Camera c1, c2;
	c1.set_eye_lookat_up(math::vec<3>(1, 0, 0), math::vec<3>(2, 0, 0), math::vec<3>(0, 1, 0));
	c2.set_translation_rotation(math::vec<3>(-1, 0, 0), math::vec<3>(0, 0, 0));
	camera_views_are_equal(c1, c2);
}

BOOST_AUTO_TEST_CASE (test_view_3)
{
	graphic::Camera c1, c2;
	c1.set_eye_lookat_up(math::vec<3>(0, 1, 0), math::vec<3>(0, 0, 0), math::vec<3>(1, 0, 0));
	c2.set_translation_rotation(math::vec<3>(0, -1, 0), math::vec<3>(0, 0, -math::PI/2));
	camera_views_are_equal(c1, c2);
}

BOOST_AUTO_TEST_CASE (test_view_4)
{
	graphic::Camera c1, c2;
	c1.set_eye_lookat_up(math::vec<3>(0, 0, 0), math::vec<3>(-1, -1, 0), math::vec<3>(-sqrt(2), sqrt(2), 0));
	c2.set_translation_rotation(math::vec<3>(0, 0, 0), math::vec<3>(-math::PI / 4, 0, 0));
	camera_views_are_equal(c1, c2);
}

BOOST_AUTO_TEST_SUITE_END()
