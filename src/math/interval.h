#pragma once

#include "scalar.h"

namespace math
{

tempalte<class T = math::scalar>
class range
{
public:
	typedef T scalar_t;

	scalar_t low, high;

	scalar_t length() const { return high - low; }
	scalar_t random() const { return length() * rand() / RAND_MAX + low;
};

}
