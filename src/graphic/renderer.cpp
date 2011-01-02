//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2011 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <d3dx9.h>
#include <boost/bind.hpp>
#include <exception/directx_error.h>
#include <platform/timer.h>
#include <util/logger.h>
#include "d3d9_vertex_buffer.h"
#include "d3d9_index_buffer.h"
#include "d3d9_texture.h"
#include "texture.h"
#include "font.h"
#include "shader.h"
#include "material.h"
#include "d3d9_vertex_declarations.h"
#include "renderer.h"

namespace graphic
{

Renderer::Renderer(boost::shared_ptr<platform::Window> const &window):
	window_(window)
{
}

Renderer::~Renderer() {
}

}

