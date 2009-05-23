#pragma once

extern "C"
{
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}

namespace bootstrap
{

class lua_global {
public:
    lua_global();
    ~lua_global();

    template <class T>
    void register_class()
    {
        lua_newtable(L_);
        int table = lua_gettop(L_);

        luaL_newmetatable(L_, T::CLASS_NAME);
        int metatable = lua_gettop(L_);

        lua_pushstring(L_, T::CLASS_NAME);
        lua_pushvalue(L_, table);
        lua_settable(L_, LUA_GLOBALSINDEX);

        lua_pushstring(L_, "__metatable");
        lua_pushvalue(L_, table);
        lua_settable(L_, metatable);

        lua_pushstring(L_, "__index");
        lua_pushvalue(L_, table);
        lua_settable(L_, metatable);

        lua_pushstring(L_, "new");
        lua_pushcfunction(L_, instantiate_class<T>);
        lua_settable(L_, metatable);

        lua_pushstring(L_, "__gc");
        lua_pushcfunction(L_, destroy_class_instance<T>);
        lua_settable(L_, metatable);

        lua_pop(L_, 2);
    }

    void run();

    static lua_global &instance();

private:
    lua_State *L_;

    template <class T>
    static int instantiate_class(lua_State *L)
    {
        void *buffer = lua_newuserdata (L, sizeof(T));
        new (buffer) T(L);
        int table = lua_gettop(L);

        luaL_getmetatable(L, T::CLASS_NAME);
        lua_settable(L, table);

        return 1;
    }

    template <class T>
    static int destroy_class_instance(lua_State *L)
    {
        reinterpret_cast<T *>(lua_touserdata(L, -1))->~T();
        return 0;
    }
};

}
