#pragma once

#include <lua.h>
#include <graphic/model.h>
#include <graphic/memory_model.h>
#include <graphic/static_mesh.h>
#include <graphic/renderer.h>

namespace graphic {

class StaticModel: public Model {
public:
	StaticModel(Renderer *renderer_ptr);
	~StaticModel();

	Mesh *create_mesh();

    static void bind(lua_State *L);
};

}

