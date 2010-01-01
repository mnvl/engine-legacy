#pragma once

#include <math/vec.h>
#include <math/matrix.h>

namespace graphic
{
namespace ps
{

template<class P>
class starting_position_point
{
public:
	typedef P particle_t;

	starting_position_point(math::matrix<4,4> const &local_matrix):
		local_matrix_(local_matrix),
		matrix_(local_matrix)
	{
	}

	void update_matrix(math::matrix<4,4> const &particle_system_matrix)
	{
		math::mul(matrix_, local_matrix_, particle_system_matrix);
	}

	void construct(particle_t *particle)
	{
		math::mul(particle->position, math::vec<3>(0, 0, 0), local_matrix_);
	}

protected:
	math::matrix<4,4> const local_matrix_;
	math::matrix<4,4> matrix_;
};

}
}
