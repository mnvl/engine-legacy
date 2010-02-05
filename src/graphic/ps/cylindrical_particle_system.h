#pragma once
//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------
#include "cylindrical_emitter.h"
#include "regular_timespan_controller.h"
#include "basic_keeper.h"
#include "billboard_drawer.h"
#include "particle_system.h"

namespace graphic
{
namespace ps
{

typedef ParticleSystem<CylindricalEmitter, RegularTimespanController, BasicKeeper, BillboardDrawer> CylindricalParticleSystem;

}
}
