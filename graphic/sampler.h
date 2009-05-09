#pragma once

#include <string>
#include <vector>
#include <math/vec.h>
#include <math/quaternion.h>
#include <math/matrix.h>
#include <graphic/joint.h>

namespace engine {
namespace graphic {

struct sampler {
private:
	struct sample {
		math::scalar time;
		math::matrix<4,4> transform;
		math::quaternion<> rotation;
		math::quaternion_slerper<> rotation_interpolator;
		math::vec<3> translation;

		sample(math::scalar t = 0):
		time(t)
		{
		}
	};

	typedef std::vector<sample> samples_type;

	std::string sid_;
	joint *joint_ptr_;
	samples_type samples_;

public:
	sampler():
	joint_ptr_(0)
	{
	}

	void set_target(joint *joint_ptr, std::string const &sid) { joint_ptr_ = joint_ptr; sid_ = sid; }

	void add_sample(math::scalar time, math::matrix<4,4> const &m);
	math::matrix<4,4> get(math::scalar time) const;

	void set_target_matrix(math::scalar t) const {
		if (joint_ptr_)	joint_ptr_->get_transform_by_sid(sid_) = get(t);
	}
};


}
}

