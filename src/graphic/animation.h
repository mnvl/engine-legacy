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

#include <vector>
#include <boost/shared_ptr.hpp>
#include "sampler.h"

namespace graphic
{

typedef boost::shared_ptr<struct Animation> AnimationPtr;

struct Animation {
	typedef std::vector<Sampler> samplers_type;

	std::string name;
	samplers_type samplers;

	void set_time(math::scalar t) {
		for (samplers_type::iterator it = samplers.begin(); it != samplers.end(); it++) {
			it->set_target_matrix(t);
		}
	}
};

}
