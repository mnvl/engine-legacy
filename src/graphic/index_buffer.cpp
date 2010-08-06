//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <boost/shared_ptr.hpp>
#include <luabind/luabind.hpp>
#include "index_buffer.h"

namespace graphic
{

IndexBuffer::~IndexBuffer()
{
}

void IndexBuffer::bind(lua_State *L)
{
	using namespace luabind;

	module_(L, "graphic")
	[
		class_<IndexBuffer, boost::shared_ptr<IndexBuffer> >("IndexBuffer")
	];
}

}
