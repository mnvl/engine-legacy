//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2011 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include "d3d9_shader.h"

namespace graphic
{

class D3D9Shader::D3D9Parameter: public Parameter {
public:
	BasicParameter(boost::shared_ptr<D3D9Shader> const &shader, std::string const &name):
		shader_(shader),
		name_(name),
		handle_(0)
	{
		handle_ = shader_->get()->GetParameterByName(0, name_.c_str());
		if (!handle_) throw std::logic_error("can't get " + name_ + " parameter handle");
	}

	~BasicParameter()
	{
	}

	template <class T>
	inline void set_i(T const &value)
	{
		set(&value, sizeof(value));
	}

	virtual void set(math::vec<2> const &v) { set_i(v); }
	virtual void set(math::vec<3> const &v) { set_i(v); }
	virtual void set(math::vec<4> const &v) { set_i(v); }

	virtual void set(math::matrix<4,4> const &value)
	{
		HRESULT hr = shader_->get()->SetMatrix(handle_, (D3DXMATRIX const *) value.ij);
		if (FAILED(hr)) throw exception::directx_error(hr);
	}

	virtual void set(std::vector<math::matrix<4,4> > const &value)
	{
		HRESULT hr = shader_->get()->SetMatrixArray(handle_, (D3DXMATRIX const *) &value[0], value.size());
		if (FAILED(hr)) throw exception::directx_error(hr);
	}

	virtual void set(TexturePtr const &texture)
	{
		HRESULT hr = shader_->get()->SetTexture(handle_, texture ? static_cast<D3D9Texture *>(texture.get())->get() : 0);
		if (FAILED(hr)) throw exception::directx_error(hr);
	}

	void set(void const *ptr, size_t size)
	{
		HRESULT hr = shader_->get()->SetValue(handle_, ptr, size);
		if (FAILED(hr)) throw exception::directx_error(hr);
	}

private:
	boost::shared_ptr<D3D9Shader> const shader_;
	std::string const name_;
	D3DXHANDLE handle_;
};

class Shader::D3D9Technique: public Shader::Technique {
public:
	Technique(boost::shared_ptr<D3D9Shader> const &shader, std::string const &name):
		shader_(shader),
		name_(name),
		handle_(0)
	{
		handle_ = shader_->get()->GetTechniqueByName(name_.c_str());
		if (!handle_) throw std::logic_error("can't get " + name_ + " technique handle");
	}

	~Technique()
	{
	}

	void set()
	{
		HRESULT hr = shader_->get()->SetTechnique(handle_);
		if (FAILED(hr)) throw exception::directx_error(hr);
	}

private:
	boost::shared_ptr<D3D9Shader> const shader_;
	std::string const name_;
	D3DXHANDLE handle_;
};

D3D9Shader::~D3D9Shader()
{
}

void D3D9Shader::load_from_file(std::string const &filename)
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

void D3D9Shader::load_from_buffer(char const *buffer, size_t len)
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

void D3D9Shader::log_compilation_errors(ID3DXBuffer *compilation_errors_ptr)
{
	if (compilation_errors_ptr && compilation_errors_ptr->GetBufferSize() > 0)
	{
		LOG_WARN("compilation messages:" << std::string((char const *) compilation_errors_ptr->GetBufferPointer(),
				compilation_errors_ptr->GetBufferSize()));
		compilation_errors_ptr->Release();
	}
}

void D3D9Shader::postconstruct()
{
}

void D3D9Shader::handle_lost_device()
{
	HRESULT hr = effect_->OnLostDevice();
	if (FAILED(hr)) throw exception::directx_error(hr);
}

void D3D9Shader::handle_reset_device()
{
	HRESULT hr = effect_->OnResetDevice();
	if (FAILED(hr)) throw exception::directx_error(hr);
}

ParameterPtr D3D9Shader::get_parameter(std::string const &name)
{
	return new D3D9Parameter(shared_from_this(), name);
}

TechniquePtr D3D9Shader::get_technique(std::string const &name)
{
	return new D3D9Technique(shared_from_this(), name);
}

D3D9Shader(D3D9RendererPtr const &renderer_ptr):
	renderer_(renderer_ptr)
{
}

}

