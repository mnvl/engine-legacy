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

#include <boost/scoped_ptr.hpp>
#include "technique.h"
#include "basic_shader.h"
#include "point_light_shader.h"
#include "normal_map_shader.h"

namespace graphic
{
namespace shader
{

class StaticModelShader: public BasicShader, public PointLightShader, public NormalMapShader
{
public:
	StaticModelShader(boost::shared_ptr<Renderer> renderer_ptr);
	~StaticModelShader();

	boost::scoped_ptr<Technique> basic_tech;
	boost::scoped_ptr<Technique> normal_map_tech;

	virtual void postconstruct();
};

}
}
