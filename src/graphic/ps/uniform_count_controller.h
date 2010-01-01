#pragma once

#include <math/interval.h>

namespace graphic
{
namespace ps
{

class uniform_count_controller
{
public:
	uniform_count_controller(size_t count_limit, math::interval<> const &emissions_per_sec):
		count_limit_(count_limit),
		emissions_per_(emissions_per_sec),
		next_emission_time_left_(0)
	{
	}

	size_t move(math::scalar dt)
	{
		size_t count = 0;

		while (next_emission_time_left_ < dt)
		{
			count++;
			next_emission_time_left_ = -dt + 1 / emissions_per_sec_.random();
		}

		return count;
	}

private:
	size_t count_limit_;
	math::interval<> const emissions_per_sec_;
	math::scalar next_emission_time_left_;
};

}
}
