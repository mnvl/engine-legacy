#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <boost/intrusive_ptr.hpp>
#include <util/boost_com_intrusive_ptr.h>
#include <exception/directx_error.h>
#include <graphic/renderer.h>

namespace graphic
{

class texture {
public:
	texture(renderer *renderer_ptr):
		renderer_ptr_(renderer_ptr)
	{
	}

	void load_from_file(std::string const &filename)
	{
		filename_ = filename;

		IDirect3DTexture9 *texture_ptr;
		HRESULT hr = D3DXCreateTextureFromFile(renderer_ptr_->get_device(), filename.c_str(), &texture_ptr);
		if (FAILED(hr)) throw exception::directx_error(hr);
		texture_ptr_.reset(texture_ptr);
		texture_ptr->Release();
	}

	std::string const &get_filename() const
	{
		return filename_;
	}

	IDirect3DTexture9 *get() const
	{
		return texture_ptr_.get();
	}

private:
	renderer *renderer_ptr_;
	std::string filename_;
	boost::intrusive_ptr<IDirect3DTexture9> texture_ptr_;
};

}
