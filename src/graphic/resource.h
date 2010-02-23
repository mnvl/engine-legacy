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

#include <deque>
#include <boost/noncopyable.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <luabind/lua_include.hpp>

namespace graphic
{

class Resource: public boost::noncopyable
{
public:
	virtual ~Resource();

	virtual void handle_lost_device() = 0;
	virtual void handle_reset_device() = 0;

	static void bind(lua_State *L);
};

}
