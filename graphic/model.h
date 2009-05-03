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
		struct transform {
			transform(std::string const &s, math::matrix<4,4> const &m):
			sid(s), matrix(m)
			{
			}

			std::string sid;
			math::matrix<4,4> matrix;
		};

		std::string id, name;
		joint *parent;
		std::vector<joint *> childs;
		std::vector<transform> transforms;
		math::matrix<4,4> frame_transform;

		joint():
		parent(0) {
		}

		math::matrix<4,4> &get_transform_by_sid(std::string const &sid) {
			for (std::vector<transform>::iterator it = transforms.begin(); it != transforms.end(); it++)
				if (it->sid == sid) return it->matrix;
			throw std::logic_error("transform sid = " + sid + " not found");
		}

		void build_frame_transforms() {
			frame_transform.identity();

			for (std::vector<transform>::reverse_iterator it = transforms.rbegin(); it != transforms.rend(); it++)
				frame_transform *= it->matrix;

			if (parent) frame_transform *= parent->frame_transform;

			for (std::vector<joint *>::iterator it = childs.begin(); it != childs.end(); it++)
				(*it)->build_frame_transforms();
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
			math::quaternion_slerper<> rotation_interpolator;
			math::vec<3> translation;

			sample(math::scalar t = 0):
			time(0)
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

	struct animation {
		typedef std::vector<sampler> samplers_type;

		std::string name;
		samplers_type samplers;

		void set_time(math::scalar t) {
			for (samplers_type::iterator it = samplers.begin(); it != samplers.end(); it++) {
				it->set_target_matrix(t);
			}
		}
	};
};

}
}