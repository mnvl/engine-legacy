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
#include "shaders_library.h"

namespace graphic
{

ShadersLibrary::ShadersLibrary(Renderer * const renderer_ptr):
    renderer_(renderer_ptr)
{
}

ShadersLibrary::~ShadersLibrary()
{
}

boost::shared_ptr<MeshShader> ShadersLibrary::get_mesh_shader()
{
	boost::shared_ptr<MeshShader> shader = mesh_shader_.lock();
	if (shader) return shader;

	shader.reset(new MeshShader(renderer_->shared_from_this()));
	renderer_->register_resource(boost::static_pointer_cast<Resource>(shader));

	shader->load_from_file(util::get_graphic_sources_path() + "mesh.fx");

	mesh_shader_ = shader;

	shader_parameters::POINT_LIGHTS pl;
	memset(&pl, 0, sizeof(pl));
	pl.items[0].position.set(0, 1, 0);
	pl.items[0].color.set(1, 1, 1);
	pl.items[0].inv_attenuation = 0.95f;

	pl.items[1].position.set(0, 100000000, 0);
	pl.items[1].color.set(0.3f, 0.3f, 0.3f);
	pl.items[1].inv_attenuation = 1.0f;

	shader->point_lights->set(pl);

	return shader;
}

}
