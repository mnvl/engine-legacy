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
#include <boost/cstdint.hpp>

namespace platform
{

class Timer {
public:
	Timer();
	~Timer();

	static double get_current_time();
	static void bind(lua_State *L);

private:
	static Timer &instance();
	boost::int64_t performance_frequency_;
};

}

