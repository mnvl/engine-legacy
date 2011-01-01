//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2011 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <luabind/luabind.hpp>
#include "draw_call.h"

namespace graphic
{

void DrawCall::bind(lua_State *L)
{
	using namespace luabind;

	module_(L, "graphic")
	[
		class_<DrawCall, DrawCallPtr >("DrawCall")
		.def(constructor<RendererPtr const &>())
		.def("set_priority", &DrawCall::set_priority)
		.def("set_always_visible", &DrawCall::set_always_visible)
		.def("set_obb", &DrawCall::set_obb)
		.def("set_vertex_buffer", &DrawCall::set_vertex_buffer)
		.def("set_index_buffer", &DrawCall::set_index_buffer)
		.def("set_material", &DrawCall::set_material)
		.def("set_minimal_vertex_index", &DrawCall::set_minimal_vertex_index)
		.def("set_vertices_count", &DrawCall::set_vertices_count)
		.def("set_first_index_offset", &DrawCall::set_first_index_offset)
		.def("set_triangles_count", &DrawCall::set_triangles_count)
		.def("set_world_matrix", &DrawCall::set_world_matrix)
		.def("set_camera", &DrawCall::set_camera)
		.def("commit", &DrawCall::commit)
	];
}

}
