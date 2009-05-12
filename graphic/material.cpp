
#include <exception/directx_error.h>
#include <graphic/material.h>

namespace graphic {

material::material(renderer *renderer_ptr):
renderer_ptr_(renderer_ptr)
{
	properties_.Ambient.r = properties_.Ambient.g = properties_.Ambient.b = properties_.Ambient.a = 1.0f;
	properties_.Diffuse = properties_.Ambient;
	properties_.Specular = properties_.Ambient;
	properties_.Emissive = properties_.Ambient;
	properties_.Power = 1.0f;
}

material::~material()
{
}

void material::set_texture(std::string const &filename)
{
	texture_name_ = filename;

	if (renderer_ptr_)
	{
		IDirect3DTexture9 *texture_ptr;
		HRESULT hr = D3DXCreateTextureFromFile(renderer_ptr_->get_device(), filename.c_str(), &texture_ptr);
		if (FAILED(hr)) throw exception::directx_error(hr);
		texture_ptr_.reset(texture_ptr);
		texture_ptr->Release();
	}
}

void material::set_specular_map(std::string const &filename)
{
	specular_map_name_ = filename;

	if (renderer_ptr_)
	{
		IDirect3DTexture9 *texture_ptr;
		HRESULT hr = D3DXCreateTextureFromFile(renderer_ptr_->get_device(), filename.c_str(), &texture_ptr);
		if (FAILED(hr)) throw exception::directx_error(hr);
		texture_ptr_.reset(texture_ptr);
		texture_ptr->Release();
	}
}

void material::select()
{
	HRESULT hr;

	hr = renderer_ptr_->get_device()->SetTexture(0, texture_ptr_.get());
	if (FAILED(hr)) throw exception::directx_error(hr);

	hr = renderer_ptr_->get_device()->SetMaterial(&properties_);
	if (FAILED(hr)) throw exception::directx_error(hr);
}

}

