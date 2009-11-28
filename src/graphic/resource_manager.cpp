
#include <exception/directx_error.h>
#include <graphic/font.h>
#include <graphic/material.h>
#include <graphic/shader.h>
#include <graphic/renderer.h>
#include <graphic/resource_manager.h>

namespace graphic
{

static char ANIMATED_MODEL_FX[] = {
    #include "animated_model.dump.h"
};

// TODO: fix cyclic references renderer -> resource_manager -> resources -> renderer
ResourceManager::ResourceManager(Renderer * const renderer_ptr):
    renderer_ptr_(renderer_ptr)
{
    default_font_.reset(new Font(renderer_ptr_, "Courier New"));
	default_material_.reset(new graphic::Material(renderer_ptr_->shared_from_this()));

    {
        IDirect3DVertexDeclaration9 *decl_ptr;
        HRESULT hr = renderer_ptr_->get_device()->CreateVertexDeclaration(vertex_traits<vertex_pnt>::get_descr(), &decl_ptr);
        if (FAILED(hr)) throw exception::directx_error(hr);
        pnt_decl_.reset(decl_ptr);
    }

    {
        IDirect3DVertexDeclaration9 *decl_ptr;
        HRESULT hr = renderer_ptr_->get_device()->CreateVertexDeclaration(vertex_traits<vertex_pnjwt>::get_descr(), &decl_ptr);
        if (FAILED(hr)) throw exception::directx_error(hr);
        pnjwt_decl_.reset(decl_ptr);
    }

    animated_model_shader_ptr_.reset(new Shader(renderer_ptr_->shared_from_this()));
    animated_model_shader_ptr_->load_from_buffer(ANIMATED_MODEL_FX, sizeof(ANIMATED_MODEL_FX));
	//animated_model_shader_ptr_->load_from_file("../src/graphic/animated_model.fx");
}

ResourceManager::~ResourceManager()
{
}

}
