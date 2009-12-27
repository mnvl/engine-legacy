
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
