
#include <iostream>
#include <ctime>
#include <boost/lexical_cast.hpp>

namespace std
{
	using ::clock_t;
//	using ::clock;
}

//#define LUABIND_NO_ERROR_CHECKING
//#define LUABIND_DONT_COPY_STRINGS
//#define LUABIND_NOT_THREADSAFE

#include "lua.h"
#include "lauxlib.h"

#include <luabind/luabind.hpp>

// luabind function
float f1(int a, float b, const char* str)
{
	return 3.14f;
}

// empty function
int f2(lua_State* L)
{
	int a = (int) lua_tonumber(L, 1);
	float b = (float) lua_tonumber(L, 2);
	const char *str = lua_tostring(L, 3);

	lua_pushnumber(L, 3.14f);

	return 1;
}

struct A
{
	int x1;
	float x2;
};

using namespace luabind;

int main()
{
	static const int ncycles = 10000000;

	using namespace luabind;

	lua_State* L = lua_open();
	open(L);

	module(L)
	[
		def("test1", &f1),

		class_<A>("A")
		.def(constructor<>())
		.def_readwrite("x1", &A::x1)
		.def_readwrite("x2", &A::x2)
	];

	lua_pushstring(L, "test2");
	lua_pushcclosure(L, &f2, 0);
	lua_settable(L, LUA_GLOBALSINDEX);

	std::clock_t t1 = std::clock();
	luaL_dostring(L, "for i = 1, 10000000 do test1(5, 4.6, 'foo') end");
	std::clock_t t2 = std::clock();
	luaL_dostring(L, "for i = 1, 10000000 do test2(5, 4.6, 'foo') end");
	std::clock_t t3 = std::clock();
	luaL_dostring(L, "a = A()\nfor i = 1, 1000000 do a.x1 = i a.x2 = i end");
	std::clock_t t4 = std::clock();

	double delta1 = double(t2 - t1) / CLOCKS_PER_SEC;
	double delta2 = double(t3 - t2) / CLOCKS_PER_SEC;
	double delta3 = double(t4 - t3) / CLOCKS_PER_SEC;

	std::cout << "luabind calls: " << ncycles / delta1 << " per sec\n";
	std::cout << "naked calls: " << ncycles / delta2 << " per sec\n";
	std::cout << "member accesses: " << ncycles / delta3 << " per sec\n";

	lua_close(L);
}

