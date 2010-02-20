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

#include <luabind/lua_include.hpp>
#include <math/interval.h>
#include <math/vec.h>

namespace particle_system
{

class CylindricalEmitter {
public:
	class Particle
	{
	public:
		Particle(CylindricalEmitter *emitter):
			position_(emitter->origin),
			size_(emitter->size_interval.random()),
			velocity_(emitter->direction * emitter->velocity_interval.random()),
			acceleration_(emitter->direction * emitter->acceleration_interval.random()),
			ttl_(emitter->ttl_interval.random())
		{
			math::scalar radius = emitter->radius_interval.random();
			math::scalar angle = emitter->angle_interval.random();
			position_ += math::vec<3>(radius * math::sin(angle), 0, radius * math::cos(angle));
		}

		math::vec<3> const &get_position() const { return position_; }
		math::scalar const get_size() const { return size_; }

		void move(math::scalar dt)
		{
			velocity_ += acceleration_ * dt;
			position_ += velocity_ * dt;
			ttl_ -= dt;
		}

		bool alive()
		{
			return ttl_ > 0;
		}

	private:
		math::vec<3> position_;
		math::scalar size_;
		math::vec<3> velocity_;
		math::vec<3> acceleration_;
		math::scalar ttl_;
	};

	typedef Particle particle_t;

	math::vec<3> origin, direction;
	math::interval<> size_interval;
	math::interval<> velocity_interval;
	math::interval<> acceleration_interval;
	math::interval<> ttl_interval;
	math::interval<> radius_interval;
	math::interval<> angle_interval;

	CylindricalEmitter():
		origin(0, 0, 0),
		direction(0, 1, 0),
		size_interval(0, 1),
		velocity_interval(0, 1),
		acceleration_interval(0, 1),
		ttl_interval(0, 1),
		radius_interval(1, 1),
		angle_interval(0, math::PI * 2)
	{
	}

	static void bind(lua_State *L);
};

}
