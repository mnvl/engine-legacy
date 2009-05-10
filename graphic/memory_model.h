#pragma once

#include <boost/ptr_container/ptr_vector.hpp>
#include <math/matrix.h>
#include <graphic/memory_mesh.h>
#include <graphic/model.h>

namespace graphic {

class memory_model: public model {
public:
	void cleanup();

	memory_mesh *get_mesh_by_id(std::string const &id) {
		return (memory_mesh *) model::get_mesh_by_id(id);
	}
	
	memory_mesh *get_mesh_by_name(std::string const &name) {
		return (memory_mesh *) model::get_mesh_by_name(name);
	}

	void set_animation_time(math::scalar t);
	void draw_wireframe(renderer *renderer_ptr, math::scalar time);
};

}
