
#include <util/logger.h>
#include "vertex.h"
#include "renderer.h"
#include "uber_shader.h"

namespace graphic
{

UberShader::Instance::Instance(RendererPtr const &renderer, unsigned flags)
{
	shader_ = renderer->create_shader();

	if (flags & FLAG_VERTEX_N_ENABLED)
		shader_->add_preprocessor_macro("VERTEX_N_ENABLED");

	if (flags & FLAG_VERTEX_TB_ENABLED)
		shader_->add_preprocessor_macro("VERTEX_TB_ENABLED");

	if (flags & FLAG_VERTEX_JW_ENABLED)
		shader_->add_preprocessor_macro("VERTEX_JW_ENABLED");

	if (flags & FLAG_SKELETAL_ANIMATION_ENABLED)
		shader_->add_preprocessor_macro("SKELETAL_ANIMATION_ENABLED");

	if (flags & FLAG_LIGHTING_ENABLED)
		shader_->add_preprocessor_macro("LIGHTING_ENABLED");

	if (flags & FLAG_NORMAL_MAP_ENABLED)
		shader_->add_preprocessor_macro("NORMAL_MAP_ENABLED");

	shader_->load_from_file("uber_shader.fx");

	world_view_projection_matrix = shader_->get_parameter("world_view_projection_matrix");
	world_matrix = shader_->get_parameter("world_matrix");
	diffuse_texture = shader_->get_parameter("diffuse_texture");

	if (flags & FLAG_LIGHTING_ENABLED)
		point_lights = shader_->get_parameter("point_lights");

	if (flags & FLAG_NORMAL_MAP_ENABLED)
		normal_map_texture = shader_->get_parameter("normal_map_texture");

	if (flags & FLAG_SKELETAL_ANIMATION_ENABLED)
		joint_matrix_array = shader_->get_parameter("joint_matrix_array");
}

UberShader::Instance::~Instance()
{
}

UberShader::UberShader(RendererPtr const &renderer):
	renderer_(renderer),
	lighting_enabled_(false)
{

	build_instance(false, VERTEX_PT::FORMAT, 0);

	for (int i = 0; i <= 1; ++i)
	{
		bool lit = (i == 1);
		unsigned lit_flag = lit ? FLAG_LIGHTING_ENABLED : 0;

		build_instance(lit, VERTEX_PNT::FORMAT, lit_flag | FLAG_VERTEX_N_ENABLED);
		build_instance(lit, VERTEX_PTBNT::FORMAT, lit_flag | FLAG_VERTEX_TB_ENABLED | FLAG_VERTEX_N_ENABLED | FLAG_NORMAL_MAP_ENABLED);
		build_instance(lit, VERTEX_PNJWT::FORMAT, lit_flag | FLAG_VERTEX_N_ENABLED  | FLAG_VERTEX_JW_ENABLED | FLAG_SKELETAL_ANIMATION_ENABLED);
		build_instance(lit, VERTEX_PTBNJWT::FORMAT, lit_flag | FLAG_VERTEX_TB_ENABLED | FLAG_VERTEX_N_ENABLED | FLAG_VERTEX_JW_ENABLED | FLAG_NORMAL_MAP_ENABLED | FLAG_SKELETAL_ANIMATION_ENABLED);
	}
}

UberShader::~UberShader()
{
}

UberShader::InstancePtr const &UberShader::get_instance_for_vertex_format(GenericVertex::format_type format) const
{
	instances_type const *instances = (lighting_enabled_ ? &instances_lit_ : &instances_);
	instances_type::const_iterator it = instances->find(format);
	if (it == instances->end()) throw std::logic_error("vertex format is not supported by uber shader");
	return it->second;
}

void UberShader::build_instance(bool lit, GenericVertex::format_type format, unsigned flags)
{
	LOG_INFO("*** building uber shader, lit = " << lit << ", format = " << format << ", flags = " << flags);

	RendererPtr renderer(renderer_);
	(lit ? &instances_lit_ : &instances_)->insert(std::make_pair(format, InstancePtr(new Instance(renderer, flags))));
}

}
