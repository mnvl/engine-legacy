//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2012 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>
#include "matrix.h"

void math::bind_matrix(lua_State *L)
{
    using namespace luabind;

    module(L, "math")
    [
        class_<matrix<3,3> >("matrix3x3")
        .def(constructor<>())
		.def(constructor<matrix<3,3> const &>())
        .def("rotation", (void (matrix<3,3>::*)(vec<3> const &)) &matrix<3,3>::rotation)
        .def("rotate", (void (matrix<3,3>::*)(vec<3> const &)) &matrix<3,3>::rotate)
		.def("assign", &matrix<3,3>::operator =),

        class_<matrix<4,4> >("matrix4x4")
        .def(constructor<>())
		.def(constructor<matrix<4,4> const &>())
        .def("identity", &matrix<4,4>::identity)
        .def("translation", (void (matrix<4,4>::*)(vec<3> const &)) &matrix<4,4>::translation)
        .def("translate", (void (matrix<4,4>::*)(vec<3> const &)) &matrix<4,4>::translate)
        .def("rotation", (void (matrix<4,4>::*)(vec<3> const &)) &matrix<4,4>::rotation)
        .def("rotate", (void (matrix<4,4>::*)(vec<3> const &)) &matrix<4,4>::rotate)
        .def("scaling", (void (matrix<4,4>::*)(vec<3> const &)) &matrix<4,4>::scaling)
        .def("scale", (void (matrix<4,4>::*)(vec<3> const &)) &matrix<4,4>::scale)
        .def("direction", (void (matrix<4,4>::*)(vec<3> const &)) &matrix<4,4>::direction)
        .def("direct", (void (matrix<4,4>::*)(vec<3> const &)) &matrix<4,4>::direct)
		.def("assign", &matrix<4,4>::operator =),

		def("mul", (void (*)(vec<3> &,vec<3> const &, matrix<3,3> const &)) mul),
		def("mul", (void (*)(vec<3> &,vec<3> const &, matrix<4,4> const &)) mul),
		def("mul", (void (*)(vec<4> &,vec<4> const &, matrix<4,4> const &)) mul),
		def("mul", (void (*)(matrix<4,4> &,matrix<4,4> const &, matrix<4,4> const &)) mul)
    ];
}
