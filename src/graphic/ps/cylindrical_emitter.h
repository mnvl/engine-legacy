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

namespace graphic
{
namespace ps
{

class CylindricalEmitter {
public:
	class Particle
	{
	public:
		Particle(CylindricalEmitter *emitter):
			position_(emitter->origin_),
			size_(emitter->size_.random()),
			velocity_(emitter->direction_ * emitter->velocity_.random()),
			acceleration_(emitter->direction_ * emitter->acceleration_.random()),
			ttl_(emitter->ttl_.random()),
			radius_(emitter->radius_.random()),
			angle_(emitter->angle_.random()),
		{
			position_ += angle_
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
		math::scalar radius_;
	};

	typedef Particle particle_t;

	void set_size_interval(math::interval<> const &size)
	{
		size_ = size;
	}

	void set_origin(math::vec<3> const &origin)
	{
		origin_ = origin;
	}

	void set_direction(math::vec<3> const &direction)
	{
		direction_ = direction;
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

	void set_radius_interval(math::interval<> const &radius)
	{
		radius_ = radius;
	}

	void set_angle_interval(math::interval<> const &angle)
	{
		angle_ = angle;
	}

	void bind(lua_State *L);

protected:
	friend Particle;

	math::vec<3> origin_, direction_;
	math::interval<> size_;
	math::interval<> velocity_;
	math::interval<> acceleration_;
	math::interval<> ttl_;
	math::interval<> radius_;
	math::interval<> angle_;
};

}
}
