#pragma once
//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2011 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <luabind/lua_include.hpp>
#include "scalar.h"

namespace math
{

template<class T = scalar>
class interval
{
public:
	typedef T scalar_t;

	scalar_t low, high;

	interval()
	{
	}

	interval(scalar_t l, scalar_t h):
		low(l),
		high(h)
	{
	}

	scalar_t length() const { return high - low; }
	scalar_t random() const { return length() * rand() / RAND_MAX + low; }
};

void bind_interval(lua_State *L);

}
