#pragma once

#include <vector>
#include <math/matrix.h>

namespace graphic {

class joint;

struct joint_influence {
	struct vertex_weight {
		int vertex_index;
		math::scalar weight;
	};

	typedef std::vector<vertex_weight> vertex_weights_type;

	joint_influence():
	joint_ptr(0)
	{
	}

	joint *joint_ptr;
	math::matrix<4, 4> inverted_bind_matrix;
	std::vector<vertex_weight> vertex_weights;
};

}


