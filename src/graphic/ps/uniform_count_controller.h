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
class UniformCountController
{
public:
	typedef P particle_t;

	UniformCountController():
		next_emission_time_left_(0)
	{
	}

	void emissions_per_sec(math::interval<> const &emissions_per_sec)
	{
		emissions_per_sec_ = emissions_per_sec;
	}

	size_t move(math::scalar dt)
	{
		size_t count = 0;

		while (next_emission_time_left_ < dt)
		{
			count++;
			next_emission_time_left_ = -dt + 1 / emissions_per_sec_.random();
		}

		return count;
	}

private:
	math::interval<> emissions_per_sec_;
	math::scalar next_emission_time_left_;
};

}
}
