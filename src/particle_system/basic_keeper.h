#pragma once
//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <deque>

namespace particle_system
{

template<class P>
class BasicKeeper
{
public:
	typedef P particle_t;
	typedef std::deque<particle_t> particles_t;
	typedef typename particles_t::const_iterator iterator;

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

			if (it->alive())
			{
				++it;
			}
			else
			{
				it = particles_.erase(it);
			}
		}
	}

	size_t get_particles_count() const
	{
		return particles_.size();
	}

	iterator begin() const { return particles_.begin(); }
	iterator end() const { return particles_.end(); }

private:
	particles_t particles_;
};

}
