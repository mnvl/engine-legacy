//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
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
#include "font.h"

namespace graphic
{

Font::Font(RendererPtr const &renderer, std::string const &face_name, int height)
{
	HRESULT hr;

	ID3DXFont *font_ptr;
	hr = D3DXCreateFont(renderer->get_device(), height, 0, FW_MEDIUM, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, face_name.c_str(), &font_ptr);
	if (FAILED(hr)) throw exception::directx_error(hr);

	font_.reset(font_ptr);
	font_ptr->Release();
}

Font::~Font() {
}

void Font::draw(math::vec<2> const &left_top, math::vec<2> const &right_bottom, char const *text, unsigned flags,
	math::vec<3> const &color)
{
	RECT rc;
	rc.left = (LONG) left_top.x;
	rc.top = (LONG) left_top.y;
	rc.right = (LONG) right_bottom.x;
	rc.bottom = (LONG) right_bottom.y;

	BYTE r = BYTE(color.r * 255);
	BYTE g = BYTE(color.g * 255);
	BYTE b = BYTE(color.b * 255);

	INT status = get()->DrawText(0, text, strlen(text), &rc, flags, D3DCOLOR_XRGB(r, g, b));

	if (!status)
	{
		throw std::runtime_error("can't draw text");
	}
}

void Font::handle_lost_device()
{
	get()->OnLostDevice();
}

void Font::handle_reset_device()
{
	get()->OnResetDevice();
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
