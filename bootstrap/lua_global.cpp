
#include <platform/window.h>
#include <bootstrap/lua_global.h>

namespace bootstrap
{

lua_global::lua_global()
{
    L_ = luaL_newstate();
    luaL_openlibs(L_);
}

lua_global::~lua_global()
{
    lua_close(L_);
}

void lua_global::run()
{
    luaL_dostring(L_, "w = window::new(\"zxc\")");
}

lua_global &lua_global::instance()
{
    static lua_global singleton;
    return singleton;
}

}
