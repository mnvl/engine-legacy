#pragma once
//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <luabind/lua_include.hpp>
#include <d3d9.h>
#include <d3dx9.h>
#include <boost/shared_ptr.hpp>
#include <boost/intrusive_ptr.hpp>
#include <util/boost_com_intrusive_ptr.h>
#include <exception/directx_error.h>
#include "texture.h"

namespace graphic
{

class Renderer;

class D3D9Texture: public Texture {
	friend Renderer;

public:
	D3D9Texture(boost::shared_ptr<Renderer> const &renderer_ptr, std::string const &filename);
	~D3D9Texture();

	IDirect3DTexture9 *get() const
	{
		return texture_.get();
	}

	void handle_lost_device();
	void handle_reset_device();

private:
	boost::shared_ptr<Renderer> renderer_;
	boost::intrusive_ptr<IDirect3DTexture9> texture_;

	void load_from_file(std::string const &filename);
};

}
