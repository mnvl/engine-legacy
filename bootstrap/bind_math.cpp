
#include <luabind/lua_include.hpp>
#include <luabind/luabind.hpp>
#include <math/vec.h>
#include <math/matrix.h>

namespace bootstrap
{

void bind_math(lua_State *L)
{
    using namespace luabind;
    using namespace math;

    module(L, "math")
    [
        class_<vec<2> >("vec2f")
        .def(constructor<math::scalar, math::scalar>())
        .def_readwrite("x", &vec<2>::x)
        .def_readwrite("y", &vec<2>::y),

        class_<vec<3> >("vec3f")
        .def(constructor<math::scalar, math::scalar, math::scalar>())
        .def_readwrite("x", &vec<3>::x)
        .def_readwrite("y", &vec<3>::y)
        .def_readwrite("z", &vec<3>::z),

        class_<matrix<3,3> >("matrix3x3f"),

        class_<matrix<4,4> >("matrix4x4f")
    ];
}

}
