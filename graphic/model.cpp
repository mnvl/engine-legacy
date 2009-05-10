
#include <graphic/model.h>

namespace graphic {

model::model(renderer *renderer_ptr):
renderer_ptr_(renderer_ptr)
{
}

model::~model()
{
}

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

mesh *model::get_mesh_by_id(std::string const &id)
{
	for (meshes_type::iterator it = meshes.begin(); it != meshes.end(); it++) {
		if (it->id == id) return &*it;
	}

	throw std::runtime_error("mesh with id = " + id + " not found");
}

mesh *model::get_mesh_by_name(std::string const &name)
{
	for (meshes_type::iterator it = meshes.begin(); it != meshes.end(); it++) {
		if (it->name == name) return &*it;
	}

	throw std::runtime_error("mesh with name = " + name + " not found");
}

}
