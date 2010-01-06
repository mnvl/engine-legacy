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

#include <math/vec.h>

namespace graphic
{
namespace ps
{

struct ParticlePVAT
{
	math::vec<3> position;
	math::vec<3> velocity;
	math::vec<3> acceleration;
	math::scalar ttl;

	void move(math::scalar dt)
	{
		velocity += acceleration * dt;
		position += velocity * dt;
		ttl -= dt;
	}

	bool should_die() {
		return ttl > 0;
	}
};

}
}
