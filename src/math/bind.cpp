//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include "interval.h"
#include "vec.h"
#include "matrix.h"
#include "ray.h"
#include "plane.h"
#include "bind.h"

namespace math
{

void bind(lua_State *L)
{
	bind_interval(L);
	bind_vec(L);
	bind_matrix(L);
	bind_ray(L);
	bind_plane(L);
}

}
