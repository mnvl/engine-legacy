#pragma once
//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <boost/noncopyable.hpp>
#include <luabind/lua_include.hpp>
#include "forward.h"

namespace graphic
{

class D3D9IndexBuffer;

class IndexBuffer: public boost::noncopyable {
public:
	virtual ~IndexBuffer();

	virtual const void *lock() const = 0;
	virtual void unlock() const = 0;
	virtual void *lock(bool discard) = 0;
	virtual void unlock() = 0;

	virtual void *begin() = 0;
	virtual void const *begin() const = 0;

	static void bind(lua_State *L);
};

}
