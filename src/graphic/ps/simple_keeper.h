//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

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
