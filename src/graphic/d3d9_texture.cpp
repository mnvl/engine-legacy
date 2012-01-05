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

D3D9Texture::D3D9Texture(D3D9RendererPtr const &renderer_ptr, int width, int height, int bpp):
	Texture(width, height, bpp),
	renderer_(renderer_ptr)
{
	create_texture(width, height, bpp);
}

D3D9Texture::~D3D9Texture()
{
}

void D3D9Texture::create_texture(int width, int height, int bpp) {
	IDirect3DTexture9 *texture_ptr;
	HRESULT hr = renderer_->get_device()->CreateTexture(width, height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED, &texture_ptr, 0);
	if (FAILED(hr)) throw exception::directx_error(hr);
}


void D3D9Texture::handle_lost_device()
{
	texture_.reset();
}

void D3D9Texture::handle_reset_device()
{
}

}
