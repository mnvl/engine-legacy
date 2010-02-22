//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <exception/directx_error.h>
#include <util/get_sources_path.h>
#include "font.h"
#include "material.h"
#include "mesh_shader.h"
#include "renderer.h"
#include "shared_resources.h"

namespace graphic
{

SharedResources::SharedResources(Renderer * const renderer_ptr):
    renderer_ptr_(renderer_ptr),
	vertex_declarations_(renderer_ptr)
{
}

SharedResources::~SharedResources()
{
}

boost::shared_ptr<Font> SharedResources::get_default_font()
{
	boost::shared_ptr<Font> font = default_font_.lock();
	if (font) return font;

    font = renderer_ptr_->create_font("Courier New");
	default_font_ = font;

	return font;
}

boost::shared_ptr<Material> SharedResources::get_default_material()
{
	boost::shared_ptr<Material> material = default_material_.lock();
	if (material) return material;

	material.reset(new graphic::Material(renderer_ptr_->shared_from_this()));
	default_material_ = material;

	return material;
}

boost::shared_ptr<MeshShader> SharedResources::get_mesh_shader()
{
	boost::shared_ptr<MeshShader> shader = mesh_shader_ptr_.lock();
	if (shader) return shader;

	shader.reset(new MeshShader(renderer_ptr_->shared_from_this()));

#if defined(NDEBUG)
	shader->load_from_buffer(ANIMATED_MODEL_FX, sizeof(ANIMATED_MODEL_FX));
#else
	shader->load_from_file(util::get_graphic_sources_path() + "mesh.fx");
#endif

	mesh_shader_ptr_ = shader;

	shader_parameters::POINT_LIGHTS pl;
	memset(&pl, 0, sizeof(pl));
	pl.items[0].position.set(0, 1, 0);
	pl.items[0].color.set(1, 1, 1);
	pl.items[0].inv_attenuation = 0.95f;


//	pl.items[1].position.set(0, 100000000, 0);
//	pl.items[1].color.set(0.1f, 0.1f, 0.1f);
//	pl.items[1].inv_attenuation = 1.0f;

	shader->point_lights->set(pl);

	return shader;
}

}
