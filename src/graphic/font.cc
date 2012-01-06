//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2012 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <boost/lexical_cast.hpp>
#include <luabind/luabind.hpp>
#include "texture.h"
#include "renderer.h"
#include "font.h"

#if defined(max)
#undef max
#endif

namespace graphic
{

Font::Font(RendererPtr const &renderer, std::string const &filename, int height):
	renderer_(renderer)
{
	init_library();

	font_ = TTF_OpenFont(filename.c_str(), height);
	if (!font_) throw_error();
}

Font::~Font() {
	TTF_CloseFont(font_);
}

TexturePtr Font::create_texture(std::string const& text, math::vec<3> const &color) {
	SDL_Color sdl_color;

	sdl_color.r = Uint8(color.r * 255);
	sdl_color.g = Uint8(color.g * 255);
	sdl_color.b = Uint8(color.b * 255);

	SDL_Surface *surface = TTF_RenderUTF8_Solid(font_, text.c_str(), sdl_color);

	TexturePtr texture = Texture::load_from_sdl_surface(renderer_, surface);

	SDL_FreeSurface(surface);

	return texture;
}

void Font::draw(math::vec<2> const &left_top, math::vec<2> const &right_bottom, char const *text, unsigned flags,
	math::vec<3> const &color)
{
}

void Font::init_library() {
	struct Guard {
		Guard() {
			if (TTF_Init() == -1) {
				throw_error();
			}
		}

		~Guard() {
			TTF_Quit();
		}
	};

	static Guard guard;
}

void Font::throw_error() {
	throw std::runtime_error(std::string("SDL_ttf error ") + TTF_GetError());
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
		.def(constructor<RendererPtr const &, std::string const &, int>())
		.def("create_texture", &Font::create_texture)
		.def("draw", &Font::draw)
    ];
}

}
