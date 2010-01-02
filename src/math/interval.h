#pragma once

#include "scalar.h"

namespace math
{

template<class T = scalar>
class interval
{
public:
	typedef T scalar_t;

	scalar_t low, high;

	scalar_t length() const { return high - low; }
	scalar_t random() const { return length() * rand() / RAND_MAX + low; }
};

}
