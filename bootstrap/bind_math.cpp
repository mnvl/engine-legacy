
#include <luabind/lua_include.hpp>
#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>
#include <math/vec.h>
#include <math/matrix.h>

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

        def("add_v3f_v3f", (vec3f_t (*)(vec3f_t const &, vec3f_t const &)) operator +),
        def("sub_v3f_v3f", (vec3f_t (*)(vec3f_t const &, vec3f_t const &)) operator -),
        def("mul_v3f_f", (vec3f_t (*)(vec3f_t const &, math::scalar)) operator *),

        class_<matrix3x3f_t>("matrix3x3f"),

        class_<matrix4x4f_t>("matrix4x4f")
    ];
}

}
