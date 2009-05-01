#pragma once

#include <windows.h>
#include <boost/cstdint.hpp>
#include <exception/winapi_error.h>

namespace engine {
namespace system {

class timer {
public:
	timer() {
		LARGE_INTEGER frequency;
		if (!QueryPerformanceFrequency(&frequency))
			throw exception::winapi_error();

		performance_frequency_ = frequency.QuadPart;
	}

	~timer() {
	}

	double get_current_value() const {
		LARGE_INTEGER counter;
		if (!QueryPerformanceCounter(&counter))
			throw exception::winapi_error();

		return double(counter.QuadPart) / double(performance_frequency_);
	}

	static timer &singleton() {
		static timer instance;
		return instance;
	}

private:
	boost::int64_t performance_frequency_;
};

}
}
