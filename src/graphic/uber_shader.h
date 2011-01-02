#pragma once

//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2011 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include "shader.h"
#include "vertex_traits.h"

namespace graphic
{

class UberShader: public boost::noncopyable {
public:
	UberShader(ShaderPtr const &shader);
	virtual ~UberShader();

	// from basic.fx
	Shader::ParameterPtr world_view_projection_matrix;
	Shader::ParameterPtr world_matrix;
	Shader::ParameterPtr diffuse_texture;

	// from point_light.fx
	Shader::ParameterPtr point_lights;

	// from normal_map.fx
	Shader::ParameterPtr normal_map_texture;

	// from basic.fx
	Shader::TechniquePtr tech_pnt;
	Shader::TechniquePtr tech_ptbnt;
	Shader::TechniquePtr tech_pnjwt;
	Shader::TechniquePtr tech_ptbnjwt;

	// from skeletal_animation.fx
	Shader::ParameterPtr joint_matrix_array;

	virtual Shader::TechniquePtr get_technique_for_vertex(VertexTraits const *vertex_traits) const;

	ShaderPtr get_shader() const { return shader_; }

private:
	ShaderPtr shader_;
};

}
