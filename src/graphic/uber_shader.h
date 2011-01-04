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

#include <boost/unordered_map.hpp>
#include "forward.h"
#include "shader.h"
#include "generic_vertex.h"

namespace graphic
{

class UberShader: public boost::noncopyable {
public:
	static unsigned const FLAG_VERTEX_N_ENABLED            =  1;
	static unsigned const FLAG_VERTEX_TB_ENABLED           =  2;
	static unsigned const FLAG_VERTEX_JW_ENABLED           =  4;
	static unsigned const FLAG_SKELETAL_ANIMATION_ENABLED  =  8;
	static unsigned const FLAG_LIGHTING_ENABLED            = 16;
	static unsigned const FLAG_NORMAL_MAP_ENABLED          = 32;

	static size_t const MAX_POINT_LIGHTS = 5;

#pragma pack(push, 1)
	struct POINT_LIGHT
	{
		math::vec<3> position;
		math::vec<3> color;
		math::scalar inv_attenuation;
	};

	struct POINT_LIGHTS
	{
		POINT_LIGHT items[MAX_POINT_LIGHTS];
	};
#pragma pack(pop)

	class Instance {
	public:
		// from uber_shader.fx
		Shader::ParameterPtr world_view_projection_matrix;
		Shader::ParameterPtr world_matrix;
		Shader::ParameterPtr diffuse_texture;

		// from point_light.fx
		Shader::ParameterPtr point_lights;

		// from normal_map.fx
		Shader::ParameterPtr normal_map_texture;

		// from skeletal_animation.fx
		Shader::ParameterPtr joint_matrix_array;

		Instance(RendererPtr const &renderer, unsigned flags);
		~Instance();

		ShaderPtr const &get_shader() const { return shader_; }

	private:
		ShaderPtr shader_;
	};

	typedef boost::shared_ptr<Instance> InstancePtr;

	UberShader(RendererPtr const &renderer);
	virtual ~UberShader();

	virtual InstancePtr const &get_instance_for_vertex_format(GenericVertex::format_type format) const;
	void set_lighting_enabled(bool flag) { lighting_enabled_ = true; }

private:
	typedef boost::unordered_map<GenericVertex::format_type, InstancePtr> instances_type;

	RendererWeakPtr renderer_;

	instances_type instances_, instances_lit_;
	bool lighting_enabled_;

	void build_instance(bool lit, GenericVertex::format_type format, unsigned flags);
};

}
