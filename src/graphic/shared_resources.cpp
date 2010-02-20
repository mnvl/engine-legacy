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
#include "font.h"
#include "material.h"
#include "shader.h"
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

boost::shared_ptr<Shader> SharedResources::get_animated_model_shader()
{
	// TODO:
#if 0
	boost::shared_ptr<shader::Shader> shader = animated_model_shader_ptr_.lock();
	if (shader) return shader;

	shader.reset(new shader::Shader(renderer_ptr_->shared_from_this()));
    shader->load_from_buffer(ANIMATED_MODEL_FX, sizeof(ANIMATED_MODEL_FX));

	animated_model_shader_ptr_ = shader;

	return shader;
#endif

	return boost::shared_ptr<Shader>();
}

}
