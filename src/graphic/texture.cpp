//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2012 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <boost/shared_ptr.hpp>
#include <luabind/luabind.hpp>
#include "texture.h"

namespace graphic
{

Texture::Texture(std::string const &filename):
	filename_(filename)
{
}

Texture::~Texture()
{
}

void Texture::bind(lua_State *L)
{
	using namespace luabind;

	module(L, "graphic")
	[
		class_<Texture, TexturePtr >("Texture")
	];
}

}
