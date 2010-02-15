//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include "static_model_shader.h"

namespace graphic
{
namespace shader
{

StaticModelShader::StaticModelShader(boost::shared_ptr<Renderer> renderer_ptr):
	Shader(renderer_ptr),
	BasicShader(renderer_ptr),
	PointLightShader(renderer_ptr),
	NormalMapShader(renderer_ptr)
{
}

StaticModelShader::~StaticModelShader()
{
}

void StaticModelShader::postconstruct()
{
	BasicShader::postconstruct();
	PointLightShader::postconstruct();
	NormalMapShader::postconstruct();
}

}
}
