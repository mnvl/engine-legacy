#pragma once

#include <math/vec.h>

namespace graphic
{
namespace ps
{

struct ParticlePVAT
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

}
}
