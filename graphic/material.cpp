
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
	texture_ptr_.reset(new texture(renderer_ptr_));
	texture_ptr_->load_from_file(filename);
}

void material::select()
{
	HRESULT hr;

	hr = renderer_ptr_->get_device()->SetTexture(0, texture_ptr_->get());
	if (FAILED(hr)) throw exception::directx_error(hr);

	hr = renderer_ptr_->get_device()->SetMaterial(&properties_);
	if (FAILED(hr)) throw exception::directx_error(hr);
}

}

