//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2011 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <util/logger.h>
#include <exception/directx_error.h>
#include <graphic/renderer.h>
#include "shader.h"

#if defined(NDEBUG)
static DWORD const ENGINE_SHADER_FLAGS = D3DXSHADER_OPTIMIZATION_LEVEL3;
#else
static DWORD const ENGINE_SHADER_FLAGS = D3DXSHADER_DEBUG;
#endif

namespace graphic
{

Shader::Shader(RendererPtr const &renderer_ptr):
	renderer_(renderer_ptr)
{
}

Shader::~Shader()
{
}

void Shader::load_from_file(std::string const &filename)
{
	HRESULT hr;

	LOG_INFO("loading shader from file " << filename);

	ID3DXEffect *effect_ptr;
	ID3DXBuffer *compilation_errors_ptr;
	hr = D3DXCreateEffectFromFile(renderer_->get_device(), filename.c_str(), 0, 0, ENGINE_SHADER_FLAGS, 0,
		&effect_ptr, &compilation_errors_ptr);

	log_compilation_errors(compilation_errors_ptr);

	if (FAILED(hr)) throw exception::directx_error(hr);

	effect_.reset(effect_ptr);

	postconstruct();
}

void Shader::load_from_buffer(char const *buffer, size_t len)
{
	HRESULT hr;

	LOG_INFO("loading shader from buffer");

	ID3DXEffect *effect_ptr;
	ID3DXBuffer *compilation_errors_ptr;
	hr = D3DXCreateEffect(renderer_->get_device(), buffer, len, 0, 0, ENGINE_SHADER_FLAGS, 0,
		&effect_ptr, &compilation_errors_ptr);

	log_compilation_errors(compilation_errors_ptr);

	if (FAILED(hr)) throw exception::directx_error(hr);

	effect_.reset(effect_ptr);

	postconstruct();
}

void Shader::log_compilation_errors(ID3DXBuffer *compilation_errors_ptr)
{
	if (compilation_errors_ptr && compilation_errors_ptr->GetBufferSize() > 0)
	{
		LOG_WARN("compilation messages:" << std::string((char const *) compilation_errors_ptr->GetBufferPointer(),
				compilation_errors_ptr->GetBufferSize()));
		compilation_errors_ptr->Release();
	}
}

void Shader::postconstruct()
{
}

void Shader::handle_lost_device()
{
	HRESULT hr = effect_->OnLostDevice();
	if (FAILED(hr)) throw exception::directx_error(hr);
}

void Shader::handle_reset_device()
{
	HRESULT hr = effect_->OnResetDevice();
	if (FAILED(hr)) throw exception::directx_error(hr);
}

}

