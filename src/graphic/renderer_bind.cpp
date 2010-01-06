//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <boost/shared_ptr.hpp>
#include <luabind/luabind.hpp>
#include <platform/window.h>
#include "renderer.h"

void graphic::Renderer::bind(lua_State *L)
{
    using namespace luabind;

    module_(L, "graphic")
    [
        class_<Renderer, boost::shared_ptr<Renderer> >("Renderer")
		.def(constructor<boost::shared_ptr<platform::Window> const &>())
		.def("get_screen_width", &Renderer::get_screen_width)
		.def("get_screen_height", &Renderer::get_screen_height)
		.def("postconstruct", &Renderer::postconstruct)
		.def("begin_frame", &Renderer::begin_frame)
		.def("finish_frame", &Renderer::finish_frame)
		.def("set_view_projection_from_camera", &Renderer::set_view_projection_from_camera)
        .def("set_world_matrix", &Renderer::set_world_matrix)
		.def("create_texture", &Renderer::create_texture)
		.def("create_vertex_array_pt", &Renderer::create_vertex_array<vertex_pt>)
		.def("create_vertex_array_pnt", &Renderer::create_vertex_array<vertex_pnt>)
		.def("create_vertex_array_pnjwt", &Renderer::create_vertex_array<vertex_pnjwt>)
		.def("create_font", &Renderer::create_font)
        .def("draw_triangles", (void (Renderer::*)(VertexBuffer *, size_t))&Renderer::draw_triangles)
        .def("draw_triangles", (void (Renderer::*)(VertexBuffer *, size_t, size_t))&Renderer::draw_triangles)
		.def("disable_all_tests", &Renderer::disable_all_tests)
    ];
}

