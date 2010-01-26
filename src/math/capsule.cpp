//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include "aabb.h"
#include "obb.h"
#include "capsule.h"

namespace math
{

template capsule<3>;

template<int N, class T> void
capsule<N, T>::get_aabb(aabb<ARITY, scalar_t> &bounds) const
{
	vec_t diagonal(radius, radius, radius);

	bounds.lo = bounds.hi = axe.A;
	bounds.extend(axe.B);

	bounds.extend(axe.A - diagonal);
	bounds.extend(axe.A + diagonal);

	bounds.extend(axe.B - diagonal);
	bounds.extend(axe.B + diagonal);
}

void bind_capsule(lua_State *L)
{
	using namespace luabind;

	module(L, "math")
	[
		class_<capsule<3> >("capsule3")
		.def_readwrite("axe", &capsule<3>::axe)
		.def_readwrite("radius", &capsule<3>::radius)
	];
}

}
