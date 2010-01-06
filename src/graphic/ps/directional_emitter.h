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

#include <math/interval.h>
#include <math/vec.h>

namespace graphic
{
namespace ps
{

class DirectionalEmitter {
public:
	class Particle
	{
	public:
		Particle(DirectionalEmitter *emitter):
			position_(emitter->origin_),
			velocity_(emitter->direction_ * emitter->velocity_.random()),
			acceleration_(emitter->direction_ * emitter->acceleration_.random()),
			ttl_(emitter->ttl_.random())
		{
		}

		void move(math::scalar dt)
		{
			velocity_ += acceleration_ * dt;
			position_ += velocity_ * dt;
			ttl_ -= dt;
		}

		bool should_die()
		{
			return ttl_ > 0;
		}

	private:
		math::vec<3> position_;
		math::vec<3> velocity_;
		math::vec<3> acceleration_;
		math::scalar ttl_;
	};

	typedef Particle particle_t;

	void set_origin(math::vec<3> const &origin)
	{
		origin_ = origin;
	}

	void set_direction(math::vec<3> const &direction)
	{
		direction_ = direction_;
	}

	void set_velocity_interval(math::interval<> const &velocity)
	{
		velocity_ = velocity;
	}

	void set_acceleration_interval(math::interval<> const &acceleration)
	{
		acceleration_ = acceleration;
	}

	void set_ttl_interval(math::interval<> const &ttl)
	{
		ttl_ = ttl;
	}

protected:
	friend Particle;

	math::vec<3> origin_, direction_;
	math::interval<> velocity_;
	math::interval<> acceleration_;
	math::interval<> ttl_;
};

}
}
