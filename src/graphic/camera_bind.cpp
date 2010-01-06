//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <luabind/luabind.hpp>
#include "camera.h"

void graphic::Camera::bind(lua_State *L)
{
	using namespace luabind;

	module(L, "graphic")
	[
		class_<Camera>("Camera")
		.def(constructor<>())
		.def("set_perspective_projection", &Camera::set_perspective_projection)
		.def("set_orthographic_projection", &Camera::set_orthographic_projection)
		.def("set_eye_lookat_up", &Camera::set_eye_lookat_up)
		.def("set_translation_rotation", &Camera::set_translation_rotation)
		.def("set_view_matrix", &Camera::set_view_matrix)
		.def("get_eye_point", &Camera::get_eye_point)
		.def("get_up_dir", &Camera::get_up_dir)
		.def("get_view_dir", &Camera::get_view_dir)
		.def("get_cross_dir", &Camera::get_cross_dir)
		.def("get_inverted_view_matrix", &Camera::get_inverted_view_matrix)
		.def("get_billboard_matrix", &Camera::get_billboard_matrix)
		.def("get_inverted_projection_matrix", &Camera::get_inverted_projection_matrix)
	];
}
