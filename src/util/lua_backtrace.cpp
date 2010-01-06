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
#include "lua_backtrace.h"

namespace util
{

int make_lua_backtrace(lua_State *L)
{
	std::stringstream formatter;

	char const *error_message = lua_tostring(L, -1);
	formatter << (error_message ? error_message : "unknown error") << '\n';
	lua_pop(L, 1);

	lua_Debug d;
	int frame = 1;

	while (lua_getstack(L, frame, &d))
	{
		lua_getinfo(L, "Sln", &d);

		formatter << '\t' << frame << ' ' << d.short_src << ":" << d.currentline
			<< " " << d.namewhat;

		if (d.name) formatter << " " << d.name;

		formatter << '\n';

		frame++;
	}

	std::string const &error = formatter.str();
	lua_pushlstring(L, error.c_str(), error.size());

	return 1;
}

}
