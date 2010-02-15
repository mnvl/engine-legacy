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
	Shader(renderer_ptr),
	world_view_projection_matrix("world_view_projection_matrix"),
	world_matrix("world_matrix"),
	diffuse_texture("diffuse_texture")
{
}

BasicShader::~BasicShader()
{
}

void BasicShader::postconstruct()
{
	world_view_projection_matrix.postconstruct(this);
	world_matrix.postconstruct(this);
	diffuse_texture.postconstruct(this);
	normal_map_texture.postconstruct(this);
}

}
}
