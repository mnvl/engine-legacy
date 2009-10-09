
#include "static_model.h"
#include "collada_loader.h"
#include "renderer.h"
#include "bind.h"

namespace graphic
{

void bind(lua_State *L)
{
    StaticModel::bind(L);
    ColladaLoader::bind(L);
    Renderer::bind(L);
}

}
