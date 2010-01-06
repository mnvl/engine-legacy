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

#include <math/interval.h>

namespace graphic
{
namespace ps
{

template<class P>
class TtlLifespanController
{
public:
	typedef P particle_t;

	void init(particle_t &particle)
	{
		particle.ttl = lifespan_.random();
	}

private:
	math::interval<> lifespan_;
};

}
}
