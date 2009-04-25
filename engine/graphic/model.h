#pragma once

#include <string>
#include <boost/cstdint.hpp>
#include <math/vec.h>
#include <math/matrix.h>
#include <math/quaternion.h>
#include <vector>

namespace engine {
namespace graphic {

class model {
public:
	struct joint {
		std::string id, name;
		joint *parent;
		std::vector<joint *> childs;
		math::matrix<4,4> pretransform, interpolated_transform, posttransform;

		joint():
		parent(0) {
			pretransform.identity();
			interpolated_transform.identity();
			posttransform.identity();
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
	private:
		struct sample {
			math::scalar time;
			math::matrix<4,4> transform;
			math::quaternion<> rotation;
			math::quaternion_lerper<> rotation_lerper;
			math::vec<3> translation;

			sample(math::scalar t = 0):
			time(0)
			{
			}
		};

		typedef std::vector<sample> samples_type;

		joint *joint_ptr_;
		samples_type samples_;

	public:
		sampler():
		joint_ptr_(0)
		{
		}

		void set_joint(joint *j) { joint_ptr_ = j; }
		joint *get_joint() const { return joint_ptr_; }

		void add_sample(math::scalar time, math::matrix<4,4> const &m);
		math::matrix<4,4> get(math::scalar time) const;
	};

	struct animation {
		typedef std::vector<sampler> samplers_type;

		std::string name;
		samplers_type samplers;
	};
};

}
}