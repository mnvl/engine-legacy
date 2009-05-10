
#include <graphic/model.h>

namespace engine {
namespace graphic {

joint *model::get_joint_by_id(std::string const &id)
{
	for (joints_type::iterator it = joints.begin(); it != joints.end(); it++) {
		if (it->id == id) return &*it;
	}

	throw std::runtime_error("joint with id = " + id + " not found in model");
}

joint *model::get_joint_by_name(std::string const &name)
{
	for (joints_type::iterator it = joints.begin(); it != joints.end(); it++) {
		if (it->name == name) return &*it;
	}

	throw std::runtime_error("joint with name = " + name + " not found in model");
}

}
}