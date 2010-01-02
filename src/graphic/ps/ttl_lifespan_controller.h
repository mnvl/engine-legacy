#pragma once

#include <math/interval.h>

namespace graphic
{
namespace ps
{

template<class P>
class TtlLifespanController
{
public:
	typedef P particle_t;

	void emit(particle_t &particle)
	{
		particle.ttl = lifespan_.random();
	}

private:
	math::interval<> lifespan_;
};

}
}
