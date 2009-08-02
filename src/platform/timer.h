#pragma once

#include <windows.h>
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

	double get_current_value() const {
		LARGE_INTEGER counter;
		if (!QueryPerformanceCounter(&counter))
			throw exception::winapi_error();

		return double(counter.QuadPart) / double(performance_frequency_);
	}

	static Timer &singleton() {
		static Timer instance;
		return instance;
	}

private:
	boost::int64_t performance_frequency_;
};

}

