#pragma once

#include <boost/noncopyable.hpp>
#include <luabind/lua_include.hpp>

class LuaState
{
public:
    LuaState();
    ~LuaState();

    void dofile(std::string const &filename);
    void throw_error();

private:
    lua_State *L_;
};
