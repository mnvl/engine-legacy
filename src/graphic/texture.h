#pragma once
//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2011 Manvel Avetisian.
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
	virtual ~Texture();

	std::string const &get_filename() const
	{
		return filename_;
	}

	static void bind(lua_State *L);

protected:
	std::string filename_;
};

}
