
#include <engine/graphic/memory_model.h>

namespace engine {
namespace graphic {

memory_mesh *memory_model::get_mesh_by_name(std::string const &name) {
	for (meshes_type::iterator it = meshes_.begin(); it != meshes_.end(); it++) {
		if (it->name_ == name) return &*it;
	}

	return 0;
}

joint *memory_model::get_joint_by_name(std::string const &name) {
	for (joints_type::iterator it = joints_.begin(); it != joints_.end(); it++) {
		if (it->name == name) return &*it;
	}

	return 0;
}

}
}