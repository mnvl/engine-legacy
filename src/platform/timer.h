#pragma once

#include <windows.h>
#include <lua.h>
#include <boost/cstdint.hpp>
#include <exception/winapi_error.h>

namespace platform {

class Timer {
public:
	Timer() {
		LARGE_INTEGER frequency;
		if (!QueryPerformanceFrequency(&frequency))
			throw exception::winapi_error();

		performance_frequency_ = frequency.QuadPart;
	}

	~Timer() {
	}

	static double get_current_time() {
		LARGE_INTEGER counter;
		if (!QueryPerformanceCounter(&counter))
			throw exception::winapi_error();

		return double(counter.QuadPart) / double(instance().performance_frequency_);
	}

	static void bind(lua_State *L);

private:
	static Timer &instance() {
		static Timer instance;
		return instance;
	}

	boost::int64_t performance_frequency_;
};

}

