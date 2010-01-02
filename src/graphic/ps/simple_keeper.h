#pragma once

#include <deque>

namespace graphic
{
namespace ps
{

template<class P>
class SimpleKeeper
{
public:
	typedef P particle_t;

	void move(math::scalar dt)
	{
		particles_t::iterator it = particles_.begin();

		while (it != particles_.end())
		{
			it->move(dt);

			if (it->should_die())
			{
				it = particles_.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

private:
	typedef std::deque<particle_t> particles_t;

	particles_t particles_;
	size_t const nparticles_limit_;
};

}
}
