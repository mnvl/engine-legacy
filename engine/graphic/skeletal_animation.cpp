
#include <engine/graphic/skeletal_animation.h>

namespace engine {
namespace graphic {

math::matrix<4,4> sampler::get(math::scalar time) const
{
	samples_type::const_iterator it = samples.begin();

	while (it != samples.end() && time > it->time) {
		time -= it->time;
		it++;
	}

	if (it == samples.begin()) return it->transform;
	if (it == samples.end()) return (it - 1)->transform;

	math::scalar k = time / (it->time - (it - 1)->time);
	math::scalar inv_k = 1.0f - k;

	math::matrix<4,4> m;

	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			m.ij[i][j] = (it - 1)->transform.ij[i][j] * inv_k + it->transform.ij[i][j] * k;
		}
	}

	return m;
}

}
}