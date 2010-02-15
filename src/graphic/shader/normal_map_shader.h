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

class NormalMapShader: public virtual Shader
{
public:
	NormalMapShader(boost::shared_ptr<Renderer> renderer_ptr);
	~NormalMapShader();

	parameter::texture normal_map_texture;

	virtual void postconstruct();
};

}
}
