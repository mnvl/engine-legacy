//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include "normal_map_shader.h"

namespace graphic
{

NormalMapShader::NormalMapShader(boost::shared_ptr<Renderer> renderer_ptr):
	Shader(renderer_ptr),
	world_view_projection_matrix(this, "world_view_projection_matrix"),
	world_matrix(this, "world_matrix"),
	diffuse_texture(this, "diffuse_texture"),
	normal_map_texture(this, "normal_map_texture")
{
}

NormalMapShader::~NormalMapShader()
{
}

void NormalMapShader::postconstruct()
{
	world_view_projection_matrix.init();
	world_matrix.init();
	diffuse_texture.init();
	normal_map_texture.init();
}

}
