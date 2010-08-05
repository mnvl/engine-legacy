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
#include "texture.h"
#include "font.h"
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
		.def("create_texture", &Renderer::create_texture)
		.def("create_vertex_array_pt", &Renderer::create_vertex_array<VERTEX_PT>)
		.def("create_vertex_array_pnt", &Renderer::create_vertex_array<VERTEX_PNT>)
		.def("create_vertex_array_pnjwt", &Renderer::create_vertex_array<VERTEX_PNJWT>)
		.def("create_font", &Renderer::create_font)
    ];
}

