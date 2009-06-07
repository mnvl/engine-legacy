
#include <boost/cstdint.hpp>
#include <luabind/lua_include.hpp>
#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>
#include <math/vec.h>
#include <math/matrix.h>
#include <math/heightfield.h>

namespace bootstrap
{

void bind_math(lua_State *L)
{
    using namespace luabind;
    using namespace math;

    typedef vec<2> vec2f_t;
    typedef vec<3> vec3f_t;
    typedef vec<4> vec4f_t;
    typedef matrix<3,3> matrix3x3f_t;
    typedef matrix<4,4> matrix4x4f_t;
	typedef ray<3> ray3f_t;
    typedef heightfield<boost::uint16_t> heightfield_u16_t;

    module(L, "math")
    [
        class_<vec2f_t>("vec2f")
        .def(constructor<math::scalar, math::scalar>())
        .def_readwrite("x", &vec2f_t::x)
        .def_readwrite("y", &vec2f_t::y),

        class_<vec3f_t>("vec3f")
        .def(constructor<math::scalar, math::scalar, math::scalar>())
        .def_readwrite("x", &vec3f_t::x)
        .def_readwrite("y", &vec3f_t::y)
        .def_readwrite("z", &vec3f_t::z),

        class_<matrix3x3f_t>("matrix3x3f")
        .def(constructor<>())
        .def("identity", (void (matrix3x3f_t::*)()) &matrix3x3f_t::identity)
        .def("rotation_v3f", (void (matrix3x3f_t::*)(vec3f_t const &)) &matrix3x3f_t::rotation)
        .def("rotation_v3f_f", (void (matrix3x3f_t::*)(vec3f_t const &, math::scalar)) &matrix3x3f_t::rotation)
        .def("rotate_v3f", (void (matrix3x3f_t::*)(vec3f_t const &)) &matrix3x3f_t::rotate)
        .def("rotate_v3f_f", (void (matrix3x3f_t::*)(vec3f_t const &, math::scalar)) &matrix3x3f_t::rotate),

        class_<matrix4x4f_t>("matrix4x4f")
		.def(constructor<>())
		.def("identity", (void (matrix4x4f_t::*)()) &matrix4x4f_t::identity),

		class_<ray3f_t>("ray3f")
		.def(constructor<>())
		.def(constructor<vec3f_t const &, vec3f_t const &>()),

        class_<heightfield_u16_t>("heightfield_u16")
        .def(constructor<matrix4x4f_t const &, size_t, size_t, vec3f_t const &>())
        .def("load_from_raw_file", &heightfield_u16_t::load_from_raw_file)
        .def("y_at", &heightfield_u16_t::y_at)
		.def("trace", &heightfield_u16_t::trace),

        def("add_v3f_v3f", (vec3f_t (*)(vec3f_t const &, vec3f_t const &)) operator +),
        def("sub_v3f_v3f", (vec3f_t (*)(vec3f_t const &, vec3f_t const &)) operator -),
        def("mul_v3f_f", (vec3f_t (*)(vec3f_t const &, math::scalar)) operator *),
        def("mul_v3f_m3x3f", (vec3f_t (*)(vec3f_t const &, matrix3x3f_t const &)) operator *)
    ];
}

}
