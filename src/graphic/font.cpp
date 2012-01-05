//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2012 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <exception/directx_error.h>
#include <luabind/luabind.hpp>
#include "renderer.h"
#include "d3d9_renderer.h"
#include "font.h"

namespace graphic
{

Font::Font(RendererPtr const &renderer, std::string const &face_name, int height):
	renderer_(renderer)
{
}

Font::~Font() {
}

void Font::draw(math::vec<2> const &left_top, math::vec<2> const &right_bottom, char const *text, unsigned flags,
	math::vec<3> const &color)
{
}

void Font::bind(lua_State *L)
{
    using namespace luabind;

    module_(L, "graphic")
    [
        class_<Font, boost::shared_ptr<Font> >("Font")
		.enum_("flags")
		[
			value("FLAG_ALIGN_LEFT", FLAG_ALIGN_LEFT),
			value("FLAG_ALIGN_HCENTER", FLAG_ALIGN_HCENTER),
			value("FLAG_ALIGN_RIGHT", FLAG_ALIGN_RIGHT),
			value("FLAG_ALIGN_TOP", FLAG_ALIGN_TOP),
			value("FLAG_ALIGN_VCENTER", FLAG_ALIGN_VCENTER),
			value("FLAG_ALIGN_BOTTOM", FLAG_ALIGN_BOTTOM)
		]
		.def("draw", &Font::draw)
    ];
}

}
