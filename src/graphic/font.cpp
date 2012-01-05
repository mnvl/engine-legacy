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
#include "renderer.h"
#include "font.h"

#if defined(max)
#undef max
#endif

namespace graphic
{

Font::Font(RendererPtr const &renderer, std::string const &filename, int height):
	renderer_(renderer),
	ft_library_(get_ft_library())
{
	FT_Error error;

	error = FT_New_Face(ft_library_.get(), filename.c_str(), 0, &ft_face_);
	if (error) throw_freetype_error(error);

	error = FT_Set_Pixel_Sizes(ft_face_, 0, height);
	if (error) throw_freetype_error(error);
}

Font::~Font() {
}

TexturePtr Font::create_texture(std::string const& text, math::vec<3> const &color, math::vec<3> const &background_color) {
	FT_Error error;

	int width = ft_face_->max_advance_width * text.size();
	int height = ft_face_->max_advance_height * text.size();

	TexturePtr texture = renderer_->create_texture(

	for (size_t i = 0; i < text.size(); ++i) {
		error = FT_Load_Char(ft_face_, text[i], FT_LOAD_RENDER);
		if (error) throw_freetype_error(error);

		width += ft_face_->glyph->advance.x;
		height = std::max(height, (int) ft_face_->glyph->bitmap.rows);
	}

	int a = 0;

	return TexturePtr();
}

void Font::draw(math::vec<2> const &left_top, math::vec<2> const &right_bottom, char const *text, unsigned flags,
	math::vec<3> const &color)
{
}

boost::shared_ptr<Font::ft_library_type> Font::get_ft_library() {
	static boost::weak_ptr<ft_library_type> weak_ft_library;

	if (boost::shared_ptr<ft_library_type> result = weak_ft_library.lock())
		return result;

	FT_Library ft_library;
	FT_Error error = FT_Init_FreeType(&ft_library);
	if (error) throw_freetype_error(error);

	boost::shared_ptr<ft_library_type> result(ft_library, FT_Done_FreeType);
	weak_ft_library = result;
	return result;
}

void Font::throw_freetype_error(FT_Error error) {
	throw std::runtime_error("freetype error " + boost::lexical_cast<std::string>(error));
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
