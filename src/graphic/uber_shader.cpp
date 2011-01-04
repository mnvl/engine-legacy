
#include <util/get_sources_path.h>
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

	if (flags & FLAG_TEXTURE_BLENDING_ENABLED)
		shader_->add_preprocessor_macro("TEXTURE_BLENDING_ENABLED");

	if (flags &FLAG_TEXUTRE_BLENDING_WEIGHTS_IN_VERTEX)
		shader_->add_preprocessor_macro("TEXUTRE_BLENDING_WEIGHTS_IN_VERTEX");

	shader_->load_from_file(util::get_graphic_sources_path() + "uber_shader.fx");

	world_view_projection_matrix = shader_->get_parameter("world_view_projection_matrix");
	world_matrix = shader_->get_parameter("world_matrix");
	diffuse_texture = shader_->get_parameter("diffuse_texture_1");

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
		bool lighting_enabled = (i == 1);
		unsigned lighting_enabled_flag = lighting_enabled ? FLAG_LIGHTING_ENABLED : 0;

		build_instance(lighting_enabled,
			VERTEX_PNT::FORMAT,
			lighting_enabled_flag | FLAG_VERTEX_N_ENABLED);

		build_instance(lighting_enabled,
			VERTEX_PNTB::FORMAT,
			lighting_enabled_flag | FLAG_VERTEX_N_ENABLED | FLAG_TEXTURE_BLENDING_ENABLED
			| FLAG_TEXUTRE_BLENDING_WEIGHTS_IN_VERTEX);

		build_instance(lighting_enabled,
			VERTEX_PTBNT::FORMAT,
			lighting_enabled_flag | FLAG_VERTEX_TB_ENABLED | FLAG_VERTEX_N_ENABLED | FLAG_NORMAL_MAP_ENABLED);

		build_instance(lighting_enabled,
			VERTEX_PNJWT::FORMAT,
			lighting_enabled_flag | FLAG_VERTEX_N_ENABLED  | FLAG_VERTEX_JW_ENABLED | FLAG_SKELETAL_ANIMATION_ENABLED);

		build_instance(lighting_enabled,
			VERTEX_PTBNJWT::FORMAT,
			lighting_enabled_flag | FLAG_VERTEX_TB_ENABLED | FLAG_VERTEX_N_ENABLED | FLAG_VERTEX_JW_ENABLED
			| FLAG_NORMAL_MAP_ENABLED | FLAG_SKELETAL_ANIMATION_ENABLED);
	}
}

UberShader::~UberShader()
{
}

UberShader::InstancePtr const &UberShader::get_instance(GenericVertex::format_type format,
	MaterialPtr const &material) const
{
	InstanceKey key;
	key.vertex_format = format;
	key.lighting_enabled = lighting_enabled_;
	key.texture_blending_enabled = false;
	key.texture_blending_weights_in_vertex = false;

	instances_type::const_iterator it = instances_.find(key);
	if (it == instances_.end()) throw std::logic_error("vertex format is not supported by uber shader");
	return it->second;
}

void UberShader::build_instance(bool lighting_enabled, GenericVertex::format_type format, unsigned flags)
{
	RendererPtr renderer(renderer_);

	InstanceKey key;
	key.vertex_format = format;
	key.lighting_enabled = lighting_enabled;
	key.texture_blending_enabled = false;
	key.texture_blending_weights_in_vertex = false;

	LOG_INFO("*** building uber shader, lighting_enabled = " << lighting_enabled << ", format = " << format << ", flags = " << flags);
	instances_.insert(std::make_pair(key, InstancePtr(new Instance(renderer, flags))));
}

}
