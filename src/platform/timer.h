#pragma once

#include <lua.h>
#include <boost/cstdint.hpp>

namespace platform
{

class Timer {
public:
	Timer();
	~Timer();

	static double get_current_time();
	static void bind(lua_State *L);

private:
	static Timer &instance();
	boost::int64_t performance_frequency_;
};

}

