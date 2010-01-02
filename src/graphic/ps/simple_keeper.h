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

	void add_particle(particle_t const &particle)
	{
		particles_.push_back(particle);
	}

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

	size_t get_particles_count() const {
		return particles_.size();
	}

private:
	typedef std::deque<particle_t> particles_t;
	particles_t particles_;
};

}
}
