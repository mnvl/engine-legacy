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
#include <graphic/font.h>
#include <graphic/material.h>
#include <graphic/shader.h>
#include <graphic/renderer.h>
#include <graphic/shared_resources.h>

namespace graphic
{

static char ANIMATED_MODEL_FX[] = {
    #include "animated_model.fx.h"
};

SharedResources::SharedResources(Renderer * const renderer_ptr):
    renderer_ptr_(renderer_ptr)
{
    {
        IDirect3DVertexDeclaration9 *decl_ptr;
        HRESULT hr = renderer_ptr_->get_device()->CreateVertexDeclaration(vertex_traits<vertex_pt>::get_descr(), &decl_ptr);
        if (FAILED(hr)) throw exception::directx_error(hr);
        pt_decl_.reset(decl_ptr);
    }

    {
        IDirect3DVertexDeclaration9 *decl_ptr;
        HRESULT hr = renderer_ptr_->get_device()->CreateVertexDeclaration(vertex_traits<vertex_pnt>::get_descr(), &decl_ptr);
        if (FAILED(hr)) throw exception::directx_error(hr);
        pnt_decl_.reset(decl_ptr);
    }

    {
        IDirect3DVertexDeclaration9 *decl_ptr;
        HRESULT hr = renderer_ptr_->get_device()->CreateVertexDeclaration(vertex_traits<vertex_ptbnt>::get_descr(), &decl_ptr);
        if (FAILED(hr)) throw exception::directx_error(hr);
        ptbnt_decl_.reset(decl_ptr);
    }

    {
        IDirect3DVertexDeclaration9 *decl_ptr;
        HRESULT hr = renderer_ptr_->get_device()->CreateVertexDeclaration(vertex_traits<vertex_pnjwt>::get_descr(), &decl_ptr);
        if (FAILED(hr)) throw exception::directx_error(hr);
        pnjwt_decl_.reset(decl_ptr);
    }
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
	boost::shared_ptr<Shader> shader = animated_model_shader_ptr_.lock();
	if (shader) return shader;

	shader.reset(new Shader(renderer_ptr_->shared_from_this()));
    shader->load_from_buffer(ANIMATED_MODEL_FX, sizeof(ANIMATED_MODEL_FX));

	animated_model_shader_ptr_ = shader;

	return shader;
}

}
