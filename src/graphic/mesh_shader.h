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
#include "vertex.h"
#include "shader.h"
#include "shader_parameters.h"

namespace graphic
{

class MeshShader: public Shader
{
public:
	MeshShader(boost::shared_ptr<Renderer> const &renderer_ptr);
	~MeshShader();

	virtual void postconstruct();

	// from basic.fx
	boost::scoped_ptr<shader_parameters::float4x4> world_view_projection_matrix;
	boost::scoped_ptr<shader_parameters::float4x4> world_matrix;
	boost::scoped_ptr<shader_parameters::texture> diffuse_texture;

	// from point_light.fx
	boost::scoped_ptr<shader_parameters::point_lights> point_lights;

	// from normal_map.fx
	boost::scoped_ptr<shader_parameters::texture> normal_map_texture;

	// from basic.fx
	boost::scoped_ptr<shader_parameters::Technique> tech_pnt;
	boost::scoped_ptr<shader_parameters::Technique> tech_ptbnt;
	boost::scoped_ptr<shader_parameters::Technique> tech_pnjwt;
	boost::scoped_ptr<shader_parameters::Technique> tech_ptbnjwt;

	template<class T> shader_parameters::Technique *get_technique_for_vertex();
	template<> shader_parameters::Technique *get_technique_for_vertex<VERTEX_PNT>() { return tech_pnt.get(); }
	template<> shader_parameters::Technique *get_technique_for_vertex<VERTEX_PTBNT>() { return tech_ptbnt.get(); }
	template<> shader_parameters::Technique *get_technique_for_vertex<VERTEX_PNJWT>() { return tech_pnjwt.get(); }
	template<> shader_parameters::Technique *get_technique_for_vertex<VERTEX_PTBNJWT>() { return tech_ptbnjwt.get(); }
};

}
