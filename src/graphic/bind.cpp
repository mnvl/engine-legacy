
#include "vertex.h"
#include "model.h"
#include "static_model.h"
#include "animated_model.h"
#include "memory_model.h"
#include "collada_loader.h"
#include "camera.h"
#include "renderer.h"
#include "bind.h"

namespace graphic
{

void bind(lua_State *L)
{
	bind_vertex(L);
    Model::bind(L);
    StaticModel::bind(L);
	AnimatedModel::bind(L);
	MemoryModel::bind(L);
	ColladaLoader::bind(L);
	Camera::bind(L);
    Renderer::bind(L);
}

}
