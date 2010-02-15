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
namespace shader
{

NormalMapShader::NormalMapShader(boost::shared_ptr<Renderer> renderer_ptr):
	Shader(renderer_ptr)
{
}

NormalMapShader::~NormalMapShader()
{
}

void NormalMapShader::postconstruct()
{
	normal_map_texture.reset(new parameter::texture(this, "normal_map_texture"));
}

}
}
