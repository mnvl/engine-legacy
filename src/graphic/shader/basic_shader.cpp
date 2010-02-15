//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include "basic_shader.h"

namespace graphic
{
namespace shader
{

BasicShader::BasicShader(boost::shared_ptr<Renderer> renderer_ptr):
	Shader(renderer_ptr)
{
}

BasicShader::~BasicShader()
{
}

void BasicShader::postconstruct()
{
	world_view_projection_matrix.reset(new parameter::float4x4(this, "world_view_projection_matrix"));
	world_matrix.reset(new parameter::float4x4(this, "world_matrix"));
	diffuse_texture.reset(new parameter::texture(this, "diffuse_texture"));
}

}
}
