
#include <exception/directx_error.h>
#include <graphic/font.h>
#include <graphic/material.h>
#include <graphic/shader.h>
#include <graphic/renderer.h>
#include <graphic/resource_manager.h>

namespace graphic
{

ResourceManager::ResourceManager(Renderer *renderer_ptr):
    renderer_ptr_(renderer_ptr)
{
    default_font_.reset(new Font(renderer_ptr_, "Courier New"));
	default_material_.reset(new graphic::Material(renderer_ptr_));

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

    animated_model_shader_ptr_.reset(new Shader(renderer_ptr));
    animated_model_shader_ptr_->load_from_file("animated_model.fx");
}

ResourceManager::~ResourceManager()
{
}

}
