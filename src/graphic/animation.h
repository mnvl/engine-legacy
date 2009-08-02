#pragma once

#include <vector>
#include <graphic/sampler.h>

namespace graphic {

struct Animation {
	typedef std::vector<Sampler> samplers_type;

	std::string name;
	samplers_type samplers;

	void set_time(math::scalar t) {
		for (samplers_type::iterator it = samplers.begin(); it != samplers.end(); it++) {
			it->set_target_matrix(t);
		}
	}
};

}
