
#include "vertex.h"
#include "uber_shader.h"

namespace graphic
{

UberShader::UberShader(ShaderPtr const &shader):
	shader_(shader),
	lighting_enabled_(false)
{
	world_view_projection_matrix = shader_->get_parameter("world_view_projection_matrix");
	world_matrix = shader_->get_parameter("world_matrix");
	diffuse_texture = shader_->get_parameter("diffuse_texture");

	point_lights = shader_->get_parameter("point_lights");

	normal_map_texture = shader_->get_parameter("normal_map_texture");

	tech_pnt = shader_->get_technique("TECH_PNT");
	tech_ptbnt = shader_->get_technique("TECH_PTBNT");
	tech_pnjwt = shader_->get_technique("TECH_PNJWT");
	tech_ptbnjwt = shader_->get_technique("TECH_PTBNJWT");
	tech_pnt_lit = shader_->get_technique("TECH_PNT_LIT");
	tech_ptbnt_lit = shader_->get_technique("TECH_PTBNT_LIT");
	tech_pnjwt_lit = shader_->get_technique("TECH_PNJWT_LIT");
	tech_ptbnjwt_lit = shader_->get_technique("TECH_PTBNJWT_LIT");


	joint_matrix_array = shader_->get_parameter("joint_matrix_array");
}

UberShader::~UberShader()
{
}

Shader::TechniquePtr UberShader::get_technique_for_vertex(VertexTraits const *vertex_traits) const
{
	if (vertex_traits == VertexTraits::get<VERTEX_PNT>()) return tech_pnt;
	if (vertex_traits == VertexTraits::get<VERTEX_PTBNT>()) return tech_ptbnt;
	if (vertex_traits == VertexTraits::get<VERTEX_PNJWT>()) return tech_pnjwt;
	if (vertex_traits == VertexTraits::get<VERTEX_PTBNJWT>()) return tech_ptbnjwt;
	throw std::logic_error("unknown vertex type");
}

}
