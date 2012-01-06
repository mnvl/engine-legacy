//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2012 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include "billboard_drawer.h"
#include "directional_emitter.h"
#include "directional_particle_system.h"
#include "cylindrical_emitter.h"
#include "cylindrical_particle_system.h"
#include "bind.h"

namespace particle_system
{

void bind(lua_State *L)
{
	BillboardDrawer::bind(L);
	DirectionalEmitter::bind(L);
	DirectionalParticleSystem::bind("DirectionalParticleSystem", L);
	CylindricalEmitter::bind(L);
	CylindricalParticleSystem::bind("CylindricalParticleSystem", L);
}

}
