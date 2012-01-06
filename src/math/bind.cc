//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2012 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include "interval.h"
#include "vec.h"
#include "aabb.h"
#include "matrix.h"
#include "contact_info.h"
#include "ray.h"
#include "line.h"
#include "plane.h"
#include "sphere.h"
#include "capsule.h"
#include "heightfield.h"
#include "bind.h"

namespace math
{

void bind(lua_State *L)
{
	bind_interval(L);
	bind_vec(L);
	bind_aabb(L);
	bind_matrix(L);
	bind_contact_info(L);
	bind_ray(L);
	bind_line(L);
	bind_plane(L);
	bind_sphere(L);
	bind_capsule(L);
	bind_heightfield(L);
}

}
