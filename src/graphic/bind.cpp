
#include "resource.h"
#include "vertex.h"
#include "vertex_array.h"
#include "texture.h"
#include "joint.h"
#include "model.h"
#include "static_model.h"
#include "animated_model.h"
#include "memory_model.h"
#include "collada_loader.h"
#include "camera.h"
#include "font.h"
#include "ps/fire_particle_system.h"
#include "renderer.h"
#include "bind.h"

namespace graphic
{

void bind(lua_State *L)
{
	Resource::bind(L);
	bind_vertex(L);
	bind_vertex_array(L);
	Texture::bind(L);
	Joint::bind(L);
    Model::bind(L);
    StaticModel::bind(L);
	AnimatedModel::bind(L);
	MemoryModel::bind(L);
	ColladaLoader::bind(L);
	Camera::bind(L);
	Font::bind(L);
    Renderer::bind(L);
	ps::FireParticleSystem::bind(L, "FireParticleSystem");
}

}
