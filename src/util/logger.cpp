//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include "logger.h"

namespace util
{

namespace
{

int log_message(lua_State *L, log4cplus::LogLevel level)
{
	log4cplus::Logger logger = log4cplus::Logger::getRoot();

	lua_Debug d;
	lua_getstack(L, 1, &d);
	lua_getinfo(L, "Sln", &d);

	if(logger.isEnabledFor(level))
	{
        log4cplus::tostringstream formatter;

		for (size_t i = 1, n = lua_gettop(L); i < n; i++)
		{
			formatter << lua_tostring(L, i);
		}

		logger.forcedLog(level, formatter.str(), d.short_src, d.currentline);
    }

	lua_pop(L, 1);

	return 0;
}

int log_debug(lua_State *L)
{
	return log_message(L, log4cplus::DEBUG_LOG_LEVEL);
}

int log_info(lua_State *L)
{
	return log_message(L, log4cplus::INFO_LOG_LEVEL);
}

int log_warn(lua_State *L)
{
	return log_message(L, log4cplus::WARN_LOG_LEVEL);
}

int log_error(lua_State *L)
{
	return log_message(L, log4cplus::ERROR_LOG_LEVEL);
}

luaL_reg const lua_functions[] = {
	{"debug", log_debug},
	{"info", log_info},
	{"warn", log_warn},
	{"error", log_error},
	{0, 0}
};

}

void bind_logger(lua_State *L)
{
	luaL_register(L, "log", lua_functions);
}

}
