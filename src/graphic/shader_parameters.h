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

#include <vector>
#include <math/vec.h>
#include <math/matrix.h>
#include <exception/directx_error.h>
#include "d3d9_texture.h"
#include "d3d9_shader.h"
#include "shader.h"

namespace graphic
{
namespace shader_parameters
{

static size_t const MAX_POINT_LIGHTS = 5;

#pragma pack(push, 1)
struct POINT_LIGHT
{
	math::vec<3> position;
	math::vec<3> color;
	math::scalar inv_attenuation;
};

struct POINT_LIGHTS
{
	POINT_LIGHT items[MAX_POINT_LIGHTS];
};
#pragma pack(pop)


}
}
