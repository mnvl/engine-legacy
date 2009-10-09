
#include <lualib.h>
#include <graphic/bind.h>
#include "main.h"
#include "lua_state.h"

LuaState::LuaState():
    L_(lua_open())
{
    luaL_openlibs(L_);
    luabind::open(L_);

    graphic::bind(L_);
    Main::bind(L_);
}

LuaState::~LuaState()
{
    lua_close(L_);
}

void LuaState::dofile(std::string const &filename)
{
    if (luaL_dofile(L_, filename.c_str()))
    {
        throw_error();
    }
}

void LuaState::throw_error()
{
    throw std::runtime_error(lua_tostring(L_, 1));
}
