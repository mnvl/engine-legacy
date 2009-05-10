
#include <exception/directx_error.h>
#include <graphic/material.h>

namespace graphic {

material::material(renderer *renderer_ptr):
renderer_ptr_(renderer_ptr)
{
	white_.Ambient.r = white_.Ambient.g = white_.Ambient.b = white_.Ambient.a = 1.0f;
	white_.Diffuse = white_.Ambient;
	white_.Specular = white_.Ambient;
	white_.Emissive = white_.Ambient;
	white_.Power = 1.0f;
}

material::~material()
{
}

void material::set_texture(char const *filename)
{
	IDirect3DTexture9 *texture_ptr;
	HRESULT hr = D3DXCreateTextureFromFile(renderer_ptr_->get_device(), filename, &texture_ptr);
	if (FAILED(hr)) throw exception::directx_error(hr);
	texture_ptr_.reset(texture_ptr);
	texture_ptr->Release();
}

void material::select()
{
	HRESULT hr = renderer_ptr_->get_device()->SetTexture(0, texture_ptr_.get());
	if (FAILED(hr)) throw exception::directx_error(hr);

	hr = renderer_ptr_->get_device()->SetMaterial(&white_);
	if (FAILED(hr)) throw exception::directx_error(hr);
}

}

