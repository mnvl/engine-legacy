#pragma once

namespace graphic
{
namespace ps
{

template<class P>
class simple_lifespan
{
public:
	typedef P particle_t;

	void emit(particle_t &particle)
	{
		particle.ttl = rand() * (lifespan_max_ - lifespan_min_) / RAND_MAX + lifespan_min_;
	}

private:
	math::scalar lifespan_min_, lifespan_max_;
};

}
}
