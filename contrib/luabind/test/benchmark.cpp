#include <iostream>
#include <ctime>

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

using namespace luabind;

int main()
{
	const int num_calls = 1000000;
	const int loops = 2;

	using namespace luabind;

	lua_State* L = lua_open();
	open(L);

	module(L)
	[
		def("test1", &f1)
	];

	lua_pushstring(L, "test2");
	lua_pushcclosure(L, &f2, 0);
	lua_settable(L, LUA_GLOBALSINDEX);

	std::clock_t total1 = 0;
	std::clock_t total2 = 0;

	for (int i = 0; i < loops; ++i)
	{
		// benchmark luabind
		std::clock_t start1 = std::clock();
		luaL_dostring(L, "for i = 1, 1000000 do\n"
							"test1(5, 4.6, 'foo')\n"
						 "end");

		std::clock_t end1 = std::clock();

		// benchmark empty binding
		std::clock_t start2 = std::clock();
		luaL_dostring(L, "for i = 1, 1000000 do\n"
							"test2(5, 4.6, 'foo')\n"
						 "end");

		std::clock_t end2 = std::clock();
		total1 += end1 - start1;
		total2 += end2 - start2;
	}


	double time1 = double(total1) / (double)CLOCKS_PER_SEC;
	double time2 = double(total2) / (double)CLOCKS_PER_SEC;

#ifdef LUABIND_NO_ERROR_CHECKING
	std::cout << "without error-checking\n";
#endif
	std::cout << "luabind:\t" << time1 * 1000000 / num_calls / loops << " microseconds per call\n"
		<< "empty:\t" << time2 * 1000000 / num_calls / loops << " microseconds per call\n"
		<< "diff:\t" << ((time1 - time2) * 1000000 / num_calls / loops) << " microseconds\n\n";

	lua_close(L);
}

