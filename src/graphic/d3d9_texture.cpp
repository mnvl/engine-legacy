//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2011 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <boost/filesystem.hpp>
#include <luabind/luabind.hpp>
#include <util/logger.h>
#include "d3d9_renderer.h"
#include "d3d9_texture.h"

namespace graphic
{

D3D9Texture::D3D9Texture(D3D9RendererPtr const &renderer_ptr, std::string const &filename):
	Texture(filename),
	renderer_(renderer_ptr)
{
	load_from_file(filename);
}

D3D9Texture::~D3D9Texture()
{
}

void D3D9Texture::load_from_file(std::string const &filename)
{
	{
		boost::filesystem::path original_file_path(filename, boost::filesystem::native);

		if (original_file_path.extension() != ".dds")
		{
			boost::filesystem::path dds_file_path = boost::filesystem::change_extension(original_file_path, ".dds");

			if (boost::filesystem::exists(dds_file_path))
			{
				filename_ = dds_file_path.native_file_string();
				LOG_INFO("overrided " << filename << " to " << filename_);
			}
			else
			{
				filename_ = filename;
			}
		}
		else
		{
			filename_ = filename;
		}
	}

	LOG_INFO("loading texture from file " << filename_);

	if (renderer_)
	{
		IDirect3DTexture9 *texture_ptr;
		HRESULT hr = D3DXCreateTextureFromFile(renderer_->get_device(), filename_.c_str(), &texture_ptr);
		if (FAILED(hr)) throw exception::directx_error(hr);
		texture_.reset(texture_ptr);
		texture_ptr->Release();
	}

#if defined(_DEBUG)
	if (texture_)
	{
		D3DSURFACE_DESC ddsd;
		HRESULT hr = texture_->GetLevelDesc(0, &ddsd);
		if (FAILED(hr)) throw exception::directx_error(hr);

		typedef unsigned char uchar;

		LOG_DEBUG("format = " << std::hex << ddsd.Format << " (" << uchar(ddsd.Format >> 24) <<
			uchar(ddsd.Format >> 16) << uchar(ddsd.Format >> 8) << uchar(ddsd.Format) << ")");
	}
#endif
}

void D3D9Texture::handle_lost_device()
{
	texture_.reset();
}

void D3D9Texture::handle_reset_device()
{
	if (renderer_)
	{
		IDirect3DTexture9 *texture_ptr;
		HRESULT hr = D3DXCreateTextureFromFile(renderer_->get_device(), filename_.c_str(), &texture_ptr);
		if (FAILED(hr)) throw exception::directx_error(hr);
		texture_.reset(texture_ptr);
		texture_ptr->Release();
	}
}

}
