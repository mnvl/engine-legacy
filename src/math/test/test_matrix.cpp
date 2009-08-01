
#define BOOST_TEST_MODULE test_matrix
#include <boost/test/included/unit_test.hpp>
#include <math/matrix.h>

BOOST_AUTO_TEST_SUITE (test_matrix)

namespace
{
void test_axis_angle(math::vec<3> const &src, math::vec<3> const &axis, math::scalar angle,
	math::vec<3> const &etalon)
{
	math::matrix<4,4> m;
	m.rotation(axis, angle);

	math::vec<3> result = src * m;

	BOOST_ASSERT ((result.length() - src.length()) < math::EPSILON);
	BOOST_ASSERT ((etalon - result).length() < math::EPSILON);
}

void test_rotation(math::vec<3> const &src, math::vec<3> const &angles, math::vec<3> const &etalon)
{
	math::matrix<4,4> m;
	m.rotation(angles);

	math::vec<3> result = src * m;

	BOOST_ASSERT ((result.length() - src.length()) < math::EPSILON);
	BOOST_ASSERT ((etalon - result).length() < math::EPSILON);
}
}

BOOST_AUTO_TEST_CASE (test_rotation_2)
{
	math::vec<3> const i(1, 0, 0), j(0, 1, 0), k(0, 0, 1);

	test_axis_angle(i, j, -math::PI / 2, k);
	test_rotation(i, -math::PI / 2 * j, k);

	test_axis_angle(j, k, -math::PI / 2, i);
	test_rotation(j, -math::PI / 2 * k, i);

	test_axis_angle(k, i, -math::PI / 2, j);
	test_rotation(k, -math::PI / 2 * i, j);
}

BOOST_AUTO_TEST_SUITE_END()
