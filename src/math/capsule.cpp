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

template capsule<>;

template<class T> void
capsule<T>::get_aabb(aabb<ARITY, scalar_t> &bounds)
{
	vec_t diagonal(radius, radius, radius);

	bounds.lo = bounds.hi = axe.A;
	bounds.extend(axe.B);

	bounds.extend(axe.A - diagonal);
	bounds.extend(axe.A + diagonal);

	bounds.extend(axe.B - diagonal);
	bounds.extend(axe.B + diagonal);
}

}
