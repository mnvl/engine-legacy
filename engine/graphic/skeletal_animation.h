#pragma once

#include <string>
#include <boost/cstdint.hpp>
#include <math/vec.h>
#include <math/matrix.h>
#include <math/quaternion.h>

namespace engine {
namespace graphic {

struct joint {
	std::string name;
	joint *parent;
	std::vector<joint *> childs;

	joint():
	parent(0) {
	}
};

struct joint_influence {
	struct vertex_weight {
		boost::uint16_t vertex_index;
		math::scalar weight;
	};

	joint *joint_ptr;
	math::matrix<4, 4> inverted_bind_matrix;
	std::vector<vertex_weight> vertex_weights;
};

struct sampler {
	struct sample {
		math::scalar time;
		math::matrix<4,4> transform;
	};

	typedef std::vector<sample> samples_type;

	joint *joint_ptr;
	samples_type samples;
};

}
}