//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

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

}
}
