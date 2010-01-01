#pragma once

#include <math/vec.h>
#include "directional_emitter.h"
#include "particle_system.h"
#include "simple_lifespan_controller.h"
#include "uniform_count_controller.h"
#include "simple_keeper.h"

namespace graphic
{
namespace ps
{

struct particle_pvat
{
	math::vec<3> position;
	math::vec<3> velocity;
	math::vec<3> acceleration;
	math::scalar ttl;

	void move(math::scalar dt)
	{
		velocity += acceleration * dt;
		position += velocity * dt;
		ttl -= dt;
	}

	bool should_die() {
		return ttl > 0;
	}
};

typedef particle_system
<
	particle_pvat,
	directional_emitter,
	simple_lifespan_controller,
	uniform_count_controller,
	simple_keeper
>
fire_particle_system;

}
}
