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

#include <luabind/lua_include.hpp>
#include <boost/noncopyable.hpp>
#include "forward.h"

namespace graphic
{

class Texture: public boost::noncopyable {
public:
	Texture(std::string const &filename);
	Texture(int width, int height, int bpp);
	virtual ~Texture();

	int get_width() const { return width_; }
	int get_height() const { return height_; }
	int get_bpp() const { return bpp_; }
	std::string const &get_filename() const { return filename_; }

	static TexturePtr load_from_file(RendererPtr const &renderer, std::string const &filename);

	static void bind(lua_State *L);

protected:
	int width_, height_, bpp_;
	std::string filename_;
};

}
