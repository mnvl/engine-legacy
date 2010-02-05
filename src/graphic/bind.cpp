//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

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
#include "renderer.h"
#include "ps/directional_emitter.h"
#include "ps/directional_particle_system.h"
#include "ps/cylindrical_emitter.h"
#include "ps/cylindrical_particle_system.h"
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
	ps::DirectionalEmitter::bind(L);
	ps::DirectionalParticleSystem::bind("DirectionalParticleSystem", L);
	ps::CylindricalEmitter::bind(L);
	ps::CylindricalParticleSystem::bind("CylindricalParticleSystem", L);
}

}
