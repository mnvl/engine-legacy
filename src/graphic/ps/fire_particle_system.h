#pragma once

#include <lua.h>
#include "particle_pvat.h"
#include "directional_emitter.h"
#include "particle_system.h"
#include "ttl_lifespan_controller.h"
#include "uniform_count_controller.h"
#include "simple_keeper.h"

namespace graphic
{
namespace ps
{

typedef ParticleSystem
<
	ParticlePVAT,
	DirectionalEmitter,
	TtlLifespanController,
	UniformCountController,
	SimpleKeeper
>
FireParticleSystem;

void bind_fire_particle_system(lua_State *L);

}
}
