#pragma once

extern "C"
{
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}

#include <string>

namespace util
{

class luna {
public:
	template <class T>
	struct thunk
	{
		char const *name;
		int (T::*method)(lua_State *L);
	};

    luna();
    ~luna();

    template <class T>
    void register_class()
    {
        lua_newtable(L_);
        int table = lua_gettop(L_);

        luaL_newmetatable(L_, T::LUNA_CLASS_NAME);
        int metatable = lua_gettop(L_);

        lua_pushstring(L_, T::LUNA_CLASS_NAME);
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
        lua_settable(L_, table);

        lua_pushstring(L_, "__gc");
        lua_pushcfunction(L_, destroy_class_instance<T>);
        lua_settable(L_, metatable);

		for (thunk<T> const *method = T::LUNA_METHODS; method->name != 0; method++)
		{
			lua_pushstring(L_, method->name);
			lua_pushlightuserdata(L_, (void *) method);
			lua_pushcclosure(L_, call_instance_method<T>, 1);
			lua_settable(L_, table);
		}

        lua_pop(L_, 2);
    }

	void execute_string(std::string const &str);

	std::string get_global_string(std::string const &name)
	{
		lua_getglobal(L_, name.c_str());
		std::string result = lua_tostring(L_, -1);
		lua_pop(L_, 1);
		return result;
	}

private:
    lua_State *L_;

    template <class T>
    static int instantiate_class(lua_State *L)
    {
        void *buffer = lua_newuserdata (L, sizeof(T));

		// to use class:new instead of class.new...
		lua_replace(L, 1);

		new (buffer) T(L);

        luaL_getmetatable(L, T::LUNA_CLASS_NAME);
        lua_setmetatable(L, 1);

        return 1;
    }

    template <class T>
    static int destroy_class_instance(lua_State *L)
    {
        reinterpret_cast<T *>(lua_touserdata(L, -1))->~T();
        return 0;
    }

	template <class T>
	static int call_instance_method(lua_State *L)
	{
		T *instance_ptr = reinterpret_cast<T *>(lua_touserdata(L, 1));
		lua_remove(L, 1);

		thunk<T> *thunk_ptr = reinterpret_cast<thunk<T> *>(lua_touserdata(L, lua_upvalueindex(1)));

		return (instance_ptr->*(thunk_ptr->method))(L);
	}
};

#define LUNA_CLASS(CLASS_NAME) \
	static char const LUNA_CLASS_NAME[]; \
	static util::luna::thunk<CLASS_NAME> LUNA_METHODS[]; \

#define LUNA_DECLARATION_BEGIN(CLASS_NAME) \
	char const CLASS_NAME::LUNA_CLASS_NAME[] = #CLASS_NAME; \
	typedef CLASS_NAME LUNA_DECLARATION_CLASS; \
	util::luna::thunk<CLASS_NAME> CLASS_NAME::LUNA_METHODS[] = { \

#define LUNA_METHOD(method) \
	{ #method, &LUNA_DECLARATION_CLASS::method }, \

#define LUNA_DECLARATION_END() \
	{ 0, 0 } \
}; \

}
