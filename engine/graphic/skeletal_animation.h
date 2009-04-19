#pragma once

#include <string>
#include <boost/cstdint.hpp>
#include <math/vec.h>
#include <math/matrix.h>
#include <math/quaternion.h>
#include <vector>

namespace engine {
namespace graphic {

struct joint {
	std::string name;
	joint *parent;
	std::vector<joint *> childs;
	mutable math::matrix<4,4> frame_transform;

	joint():
	parent(0) {
		frame_transform.identity();
	}
};

struct joint_influence {
	struct vertex_weight {
		boost::uint16_t vertex_index;
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

struct sampler {
	struct sample {
		math::scalar time;
		math::matrix<4,4> transform;
	};

	typedef std::vector<sample> samples_type;

	joint *joint_ptr;
	samples_type samples;

	math::matrix<4,4> get(math::scalar time) const;
};

struct animation {
	typedef std::vector<sampler> samplers_type;
	samplers_type samplers;
};

}
}