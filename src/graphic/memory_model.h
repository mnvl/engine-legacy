#pragma once

#include <boost/ptr_container/ptr_vector.hpp>
#include <math/matrix.h>
#include <graphic/memory_mesh.h>
#include <graphic/model.h>

namespace graphic {

class MemoryModel: public Model {
public:
	MemoryModel(Renderer *renderer_ptr = 0);

	MemoryMesh *get_mesh_by_id(std::string const &id) {
		return (MemoryMesh *) Model::get_mesh_by_id(id);
	}

	MemoryMesh *get_mesh_by_name(std::string const &name) {
		return (MemoryMesh *) Model::get_mesh_by_name(name);
	}

	Mesh *create_mesh();

	virtual void draw();
	void draw_static();
	void draw_animated();

	static void bind(lua_State *L);
};

}
