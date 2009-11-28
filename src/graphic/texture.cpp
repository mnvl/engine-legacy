
#include <boost/filesystem.hpp>
#include <luabind/luabind.hpp>
#include <util/logger.h>
#include "texture.h"

namespace graphic
{

Texture::Texture(boost::shared_ptr<Renderer> const &renderer_ptr):
	renderer_ptr_(renderer_ptr)
{
}

Texture::~Texture()
{
}

void Texture::load_from_file(std::string const &filename)
{
	boost::filesystem::path dds_file_path = boost::filesystem::change_extension(boost::filesystem::path(filename, boost::filesystem::native), ".dds");

	if (boost::filesystem::exists(dds_file_path))
	{
		filename_ = dds_file_path.native_file_string();
		LOG_INFO("loading texture from file " << filename_ << " (instead of " << filename << ")");
	}
	else
	{
		filename_ = filename;
		LOG_INFO("loading texture from file " << filename_);
	}

	if (renderer_ptr_)
	{
		IDirect3DTexture9 *texture_ptr;
		HRESULT hr = D3DXCreateTextureFromFile(renderer_ptr_->get_device(), filename_.c_str(), &texture_ptr);
		if (FAILED(hr)) throw exception::directx_error(hr);
		texture_ptr_.reset(texture_ptr);
		texture_ptr->Release();
	}
}

void Texture::select()
{
	HRESULT hr = renderer_ptr_->get_device()->SetTexture(0, texture_ptr_.get());
	if (FAILED(hr)) throw exception::directx_error(hr);
}

void Texture::bind(lua_State *L)
{
	using namespace luabind;

	module(L, "graphic")
	[
		class_<Texture>("Texture")
		.def(constructor<boost::shared_ptr<Renderer> const &>())
		.def("load_from_file", &Texture::load_from_file)
		.def("select", &Texture::select)
	];
}

}
