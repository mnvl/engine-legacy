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

#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <luabind/lua_include.hpp>

namespace graphic
{

class DrawCall;
class Renderer;

class Scheduler: public boost::noncopyable {
public:
	Scheduler(boost::shared_ptr<Renderer> const &renderer);
	~Scheduler();

	void add(boost::shared_ptr<DrawCall> const &draw_call);
	void flush();

	static void bind(lua_State *L);

private:
	typedef boost::shared_ptr<DrawCall> DrawCallPtr;
	typedef std::vector<DrawCallPtr> draw_calls_t;

	boost::shared_ptr<Renderer> const &renderer_;
	draw_calls_t draw_calls_;
};

}
