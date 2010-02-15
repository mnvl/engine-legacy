//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include "point_light_shader.h"

namespace graphic
{
namespace shader
{

PointLightShader::PointLightShader(boost::shared_ptr<Renderer> renderer_ptr):
	Shader(renderer_ptr),
	point_lights("point_lights")
{
}

PointLightShader::~PointLightShader()
{
}

void PointLightShader::postconstruct()
{
	point_lights.postconstruct(this);
}

}
}
