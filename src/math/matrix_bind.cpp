
#include "matrix.h"
#include <luabind/luabind.hpp>

void math::bind_matrix(lua_State *L)
{
    using namespace luabind;

    module(L, "math")
    [
        class_<matrix<3,3> >("matrix3x3")
        .def(constructor<>()),

        class_<matrix<4,4> >("matrix4x4")
        .def(constructor<>())
        .def("identity", &matrix<4,4>::identity)
        .def("translation", (void (matrix<4,4>::*)(vec<3> const &)) &matrix<4,4>::translation)
        .def("translate", (void (matrix<4,4>::*)(vec<3> const &)) &matrix<4,4>::translate)
    ];
}
