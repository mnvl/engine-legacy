
#include <stdexcept>
#include <util/luna.h>

namespace util
{

luna::luna()
{
    L_ = luaL_newstate();
    luaL_openlibs(L_);
}

luna::~luna()
{
    lua_close(L_);
}

void luna::execute_string(std::string const &str)
{
	if (luaL_dostring(L_, str.c_str()) != 0)
	{
		std::string error_message(lua_tostring(L_, -1));
		lua_pop(L_, 1);
		throw std::runtime_error(error_message);
	}
}

}
