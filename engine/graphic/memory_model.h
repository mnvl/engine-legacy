#pragma once

#include <engine/graphic/memory_mesh.h>
#include <engine/graphic/skeletal_animation.h>

namespace engine {
namespace graphic {

class memory_model {
public:
	typedef std::vector<memory_mesh> meshes_type;
	typedef std::vector<joint> joints_type;

	meshes_type meshes_;
	joints_type joints_;

	memory_mesh *get_mesh_by_name(std::string const &name);
	joint *get_joint_by_name(std::string const &name);
};

}
}