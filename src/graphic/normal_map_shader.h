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

#include "shader.h"
#include "shader_parameter.h"

namespace graphic
{

class NormalMapShader: public Shader
{
public:
	NormalMapShader(boost::shared_ptr<Renderer> renderer_ptr);
	~NormalMapShader();

	shader_parameter::float4x4 inverse_world_matrix;
	shader_parameter::float4x4 view_world_projection_matrix;
	shader_parameter::integer  projection_type;
	shader_parameter::float3   camera_view_direction;
	shader_parameter::texture  diffuse_texture;
	shader_parameter::texture  normal_map_texture;

	virtual void postconstruct();
};

}
