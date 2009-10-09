
#include "model.h"
#include "static_model.h"
#include "collada_loader.h"
#include "camera.h"
#include "renderer.h"
#include "bind.h"

namespace graphic
{

void bind(lua_State *L)
{
    Model::bind(L);
    StaticModel::bind(L);
    ColladaLoader::bind(L);
	Camera::bind(L);
    Renderer::bind(L);
}

}
