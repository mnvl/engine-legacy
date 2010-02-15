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

#include <math/vec.h>
#include "parameter.h"
#include "shader.h"

namespace graphic
{
namespace shader
{

class PointLightShader: public virtual Shader
{
public:
	static size_t const MAX_POINT_LIGHTS = 5;

#pragma pack(push, 1)
	struct POINT_LIGHT
	{
		math::vec<3> position;
		math::vec<4> color;
		math::scalar inv_attenuation;
	};

	struct POINT_LIGHTS
	{
		POINT_LIGHT items[MAX_POINT_LIGHTS];
	};
#pragma pack(pop)

	PointLightShader(boost::shared_ptr<Renderer> renderer_ptr);
	~PointLightShader();

	Parameter<POINT_LIGHTS> point_lights;

	virtual void postconstruct();
};

}
}
