
#include <boost/test/minimal.hpp>
#include <math/triangle.h>

int test_main(int argc, char *argv[])
{
	math::triangle<3> tri(math::vec<3>(0, 1, 0), math::vec<3>(0, 3, 1), math::vec<3>(1, 3, 0));
	math::scalar t;
	math::vec<3> v;
	bool result = tri.trace(math::ray<3>(math::vec<3>(-1, 2, 0), math::vec<3>(10, 0, 0)), t, v);
	BOOST_CHECK (result == true);
	BOOST_CHECK ((math::vec<3>(0.5f, 2, 0) - v).length_sq() < math::EPSILON);
	return 0;
}
