
#include "mesh_shader.h"

namespace graphic
{

MeshShader::MeshShader(boost::shared_ptr<Renderer> const &renderer_ptr):
	Shader(renderer_ptr)
{
}

MeshShader::~MeshShader()
{
}

void MeshShader::postconstruct()
{
	world_view_projection_matrix.reset(new shader_parameters::float4x4(this, "world_view_projection_matrix"));
	world_matrix.reset(new shader_parameters::float4x4(this, "world_matrix"));
	diffuse_texture.reset(new shader_parameters::texture(this, "diffuse_texture"));

	point_lights.reset(new shader_parameters::point_lights(this, "point_lights"));

	normal_map_texture.reset(new shader_parameters::texture(this, "normal_map_texture"));

	tech_pnt.reset(new shader_parameters::Technique(this, "TECH_PNT"));
	tech_ptbnt.reset(new shader_parameters::Technique(this, "TECH_PTBNT"));
	tech_pnjwt.reset(new shader_parameters::Technique(this, "TECH_PNJWT"));
	tech_ptbnjwt.reset(new shader_parameters::Technique(this, "TECH_PTBNJWT"));

	joint_matrix_array.reset(new shader_parameters::float4x4_array(this, "joint_matrix_array"));
}

}
