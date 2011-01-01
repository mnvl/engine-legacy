//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2011 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <windows.h>
#include <luabind/luabind.hpp>
#include <exception/winapi_error.h>
#include "timer.h"

namespace platform
{

Timer::Timer()
{
	LARGE_INTEGER frequency;
	if (!QueryPerformanceFrequency(&frequency))
		throw exception::winapi_error();

	performance_frequency_ = frequency.QuadPart;
}

Timer::~Timer()
{
}

double Timer::get_current_time()
{
	LARGE_INTEGER counter;
	if (!QueryPerformanceCounter(&counter))
		throw exception::winapi_error();

	return double(counter.QuadPart) / double(instance().performance_frequency_);
}

Timer &Timer::instance()
{
	static Timer instance;
	return instance;
}

void Timer::bind(lua_State *L)
{
	using namespace luabind;

    module_(L, "platform")
    [
		def("get_current_time", &Timer::get_current_time)
    ];
}

}
