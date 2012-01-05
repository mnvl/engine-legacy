//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2012 Manvel Avetisian.
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

D3D9Texture::D3D9Texture(D3D9RendererPtr const &renderer_ptr, int width, int height,
	unsigned pixel_format, bool dynamic, bool writeonly):
	Texture(width, height, pixel_format, dynamic, writeonly),
	renderer_(renderer_ptr),
	usage_(0),
	pool_(dynamic ? D3DPOOL_DEFAULT : D3DPOOL_MANAGED)
{
	if(dynamic) usage_ |= D3DUSAGE_DYNAMIC;
	if(writeonly) usage_ |= D3DUSAGE_WRITEONLY;

	create_texture();
}

D3D9Texture::~D3D9Texture()
{
}

void D3D9Texture::create_texture() {
	assert (pixel_format_ == PIXEL_FORMAT_R8G8B8);

	IDirect3DTexture9 *texture_ptr;
	HRESULT hr = renderer_->get_device()->CreateTexture(width, height, 1, usage_, D3DFMT_A8R8G8B8,
		pool_, &texture_ptr, 0);
	if (FAILED(hr)) throw exception::directx_error(hr);
	texture_.reset(texture_ptr);
}

void D3D9Texture::handle_lost_device()
{
	texture_.reset();
}

void D3D9Texture::handle_reset_device()
{
}

}
