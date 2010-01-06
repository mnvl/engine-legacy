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

#include <graphic/renderer.h>
#include <boost/shared_ptr.hpp>

namespace graphic
{
namespace ps
{

template
<
	class Emitter,
	template<class> class Controller,
	template<class> class Keeper,
	template<class> class Drawer
>
class ParticleSystem
{
public:
	typedef Emitter emitter_t;
	typedef typename emitter_t::particle_t particle_t;
	typedef Controller<particle_t> controller_t;
	typedef Keeper<particle_t> keeper_t;
	typedef Drawer<particle_t> drawer_t;

	ParticleSystem(boost::shared_ptr<Renderer> const &renderer, size_t const max_nparticles = 1000):
		max_nparticles_(max_nparticles),
		drawer_(renderer, max_nparticles)
	{
	}

	emitter_t &get_emitter() { return emitter_; }
	controller_t &get_controller() { return controller_; }
	keeper_t &get_keeper() { return keeper_; }
	drawer_t &get_drawer() { return drawer_; }

	void move(math::scalar dt)
	{
		size_t nparticles_to_emit = controller_.move(dt);

		for (size_t i = 0; i < nparticles_to_emit && max_nparticles_ < keeper_.get_particles_count(); i++)
		{
			keeper_.add_particle(particle_t(&emitter_));
		}

		keeper_.move(dt);
	}

	void draw()
	{
		drawer_.begin();

		for (keeper_t::iterator_t it = keeper_.begin(); it != keeper_.end(); it++)
		{
			drawer_.draw(&*it);
		}

		drawer_.end();
	}

private:
	size_t max_nparticles_;

	emitter_t emitter_;
	controller_t controller_;
	keeper_t keeper_;
	drawer_t drawer_;
};

}
}
