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

#include <math/interval.h>

namespace particle_system
{

template<class P>
class RegularTimespanController
{
public:
	typedef P particle_t;

	RegularTimespanController():
		emissions_per_sec_(50, 100),
		time_accumulated_(0),
		next_emission_time_(0)
	{
	}

	void set_emissions_per_sec(math::interval<> const &emissions_per_sec)
	{
		emissions_per_sec_ = emissions_per_sec;
	}

	size_t move(math::scalar dt)
	{
		time_accumulated_ += dt;

		size_t count = 0;

		while (next_emission_time_ < time_accumulated_)
		{
			time_accumulated_ -= next_emission_time_;
			next_emission_time_ = 1 / emissions_per_sec_.random();
			++count;
		}

		return count;
	}

private:
	math::interval<> emissions_per_sec_;
	math::scalar time_accumulated_, next_emission_time_;
};

}
