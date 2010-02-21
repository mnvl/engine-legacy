
#include "basic_shader.h"

namespace graphic
{

BasicShader::BasicShader(boost::shared_ptr<Renderer> const &renderer_ptr):
	Shader(renderer_ptr)
{
}

BasicShader::~BasicShader()
{
}

void BasicShader::postconstruct()
{
	world_view_projection_matrix.reset(new shader_parameters::float4x4(this, "world_view_projection_matrix"));
	world_matrix.reset(new shader_parameters::float4x4(this, "world_matrix"));
	diffuse_texture.reset(new shader_parameters::texture(this, "diffuse_texture"));

	point_lights.reset(new shader_parameters::point_lights(this, "point_lights"));

	normal_map_texture.reset(new shader_parameters::texture(this, "normal_map_texture"));

	// from basic.fx
	tech_pnt.reset(new shader_parameters::Technique(this, "TECH_PNT"));
	tech_ptbnt.reset(new shader_parameters::Technique(this, "TECH_PTBNT"));
	tech_pnjwt.reset(new shader_parameters::Technique(this, "TECH_PNJWT"));
	tech_ptbnjwt.reset(new shader_parameters::Technique(this, "TECH_PTBNJWT"));
}

}
