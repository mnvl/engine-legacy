
#include "logger.h"
#include "lua_backtrace.h"

namespace util
{

int print_backtrace_to_log(lua_State *L)
{
	lua_Debug d;
	lua_getstack(L, 1, &d);
	lua_getinfo(L, "Sln", &d);
	std::string err = lua_tostring(L, -1);
	lua_pop(L, 1);

	LOG_ERROR(d.short_src << ":" << d.currentline
		<< "(" << d.namewhat << " " << d.name << ") " << err);

	lua_pushstring(L, err.c_str());
	return 1;
}

}
