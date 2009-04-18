#pragma once

#include <string>
#include <boost/cstdint.hpp>
#include <math/matrix.h>

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
	struct influence {
		boost::uint16_t vertex_index;
		math::scalar weight;
	};

	joint *joint_ptr;
	math::matrix<4, 4> inverted_bind_matrix;
	std::vector<influence> influences;
};

}
}