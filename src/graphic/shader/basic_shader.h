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

#include "parameter.h"
#include "shader.h"

namespace graphic
{
namespace shader
{

class BasicShader: public virtual Shader
{
public:
	parameter::float4x4 world_view_projection_matrix;
	parameter::float4x4 world_matrix;
	parameter::texture diffuse_texture;

	virtual void postconstruct();
};

}
}
