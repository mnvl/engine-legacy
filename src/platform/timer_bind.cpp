
#include "timer.h"
#include <luabind/luabind.hpp>

void platform::Timer::bind(lua_State *L)
{
	using namespace luabind;

    module_(L, "platform")
    [
		def("get_current_time", &Timer::get_current_time)
    ];
}

