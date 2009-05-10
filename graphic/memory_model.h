#pragma once

#include <boost/ptr_container/ptr_vector.hpp>
#include <math/matrix.h>
#include <graphic/memory_mesh.h>
#include <graphic/model.h>

namespace engine {
namespace graphic {

class memory_model: public model {
public:
	typedef boost::ptr_vector<memory_mesh> meshes_type;

	meshes_type meshes;

	void cleanup();

	memory_mesh *get_mesh_by_id(std::string const &id);
	memory_mesh *get_mesh_by_name(std::string const &name);
	void set_animation_time(math::scalar t);
	void draw_wireframe(renderer *renderer_ptr, math::scalar time);
};

}
}