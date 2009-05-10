#pragma once

#include <boost/ptr_container/ptr_vector.hpp>
#include <graphic/joint.h>
#include <graphic/animation.h>
#include <graphic/mesh.h>

namespace engine {
namespace graphic {

class model {
public:
	typedef boost::ptr_vector<joint> joints_type;
	typedef boost::ptr_vector<animation> animations_type;

	joints_type joints;
	animations_type animations;

	joint *get_joint_by_id(std::string const &name);
	joint *get_joint_by_name(std::string const &name);
};

}
}