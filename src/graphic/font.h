#pragma once
//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2012 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <ft2build.h>
#include FT_FREETYPE_H
#include <boost/noncopyable.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <luabind/lua_include.hpp>
#include <math/vec.h>
#include "forward.h"

namespace graphic
{

class Font: public boost::noncopyable {
public:
	Font(RendererPtr const &renderer, std::string const &filename, int height);
	~Font();

	TexturePtr create_texture(std::string const& text, math::vec<3> const &color, math::vec<3> const &background_color);

	static unsigned const FLAG_ALIGN_LEFT = 1;
	static unsigned const FLAG_ALIGN_HCENTER = 2;
	static unsigned const FLAG_ALIGN_RIGHT = 4;
	static unsigned const FLAG_ALIGN_TOP = 8;
	static unsigned const FLAG_ALIGN_VCENTER = 16;
	static unsigned const FLAG_ALIGN_BOTTOM = 32;

	void draw(math::vec<2> const &left_top, math::vec<2> const &right_bottom, char const *text, unsigned align,
		math::vec<3> const &color);

	static void bind(lua_State *L);

private:
	typedef boost::remove_pointer<FT_Library>::type ft_library_type;
	
	RendererPtr const renderer_;
	boost::shared_ptr<ft_library_type> ft_library_;
	FT_Face ft_face_;

	static boost::shared_ptr<ft_library_type> get_ft_library();
	static void throw_freetype_error(FT_Error error);
};

}
