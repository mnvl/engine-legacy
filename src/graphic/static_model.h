#pragma once

#include <boost/shared_ptr.hpp>
#include <lua.h>
#include <graphic/model.h>
#include <graphic/memory_model.h>
#include <graphic/static_mesh.h>
#include <graphic/renderer.h>

namespace graphic {

class StaticModel: public Model {
public:
	StaticModel(boost::shared_ptr<Renderer> const &renderer_ptr = boost::shared_ptr<Renderer>());
	~StaticModel();

	MeshPtr add_mesh();

    static void bind(lua_State *L);
};

}

