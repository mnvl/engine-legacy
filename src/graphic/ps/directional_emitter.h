#pragma once

#include <math/vec.h>
#include <math/matrix.h>

namespace graphic
{
namespace ps
{

// emits particles along x axis from position (0, 0, 0)
template <class P>
class DirectionalEmitter
{
public:
	typedef P particle_t;

	void set_position(math::vec<3> const &position)
	{
		position_ = position;
	}

	void set_orientation(math::matrix<3,3> const &orientation)
	{
		direction_ = math::vec<3>(1, 0, 0) * orientation;
	}

	void init(particle_t &particle)
	{
		particle.position = position_;
		particle.velocity = direction_ * (rand() * (velocity_max_ - velocity_min_) / RAND_MAX + velocity_min_);
		particle.acceleration = direction_ * (rand() * (acceleration_max_ - acceleration_min_) / RAND_MAX + acceleration_min_);
	}

protected:
	math::vec<3> position_, direction_;
	math::scalar velocity_min_, velocity_max_;
	math::scalar acceleration_min_, acceleration_max_;
};

}
}
