#pragma once

#include <list>
#include <engine/graphic/memory_mesh.h>
#include <engine/graphic/model.h>

namespace engine {
namespace graphic {

class memory_model: public model {
public:
	typedef std::list<memory_mesh> meshes_type;
	typedef std::list<joint> joints_type;
	typedef std::list<animation> animations_type;

	meshes_type meshes;
	joints_type joints;
	animations_type animations;

	memory_mesh *get_mesh_by_name(std::string const &name);
	joint *get_joint_by_name(std::string const &name);

	void draw_wireframe(renderer *renderer_ptr, math::scalar time) const;
};

}
}