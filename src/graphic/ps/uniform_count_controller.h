#pragma once

#include <math/interval.h>

namespace graphic
{
namespace ps
{

template<class P>
class UniformCountController
{
public:
	typedef P particle_t;

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
