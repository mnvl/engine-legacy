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

#include <math/vec.h>
#include <math/matrix.h>
#include "shader.h"
#include "texture.h"

namespace graphic
{

template<class T>
class ShaderParameter
{
public:
	ShaderParameter(Shader *shader, std::string const &name):
		shader_(shader),
		name_(name),
		handle_(0)
	{
	}

	~ShaderParameter()
	{
	}

	inline void set(T const &value);

	void init()
	{
		handle_ = shader_->get()->GetParameterByName(0, name_.c_str());
		if (!handle_) throw std::logic_error("can't get " + name_ + " parameter handle");
	}

private:
	Shader *shader_;
	std::string const name_;
	D3DXHANDLE handle_;
};

void ShaderParameter<int>::set(int const &value)
{
	HRESULT hr = shader_->get()->SetValue(handle_, &value, sizeof(value));
	if (FAILED(hr)) throw exception::directx_error(hr);
}

void ShaderParameter<math::vec<3> >::set(math::vec<3> const &value)
{
	HRESULT hr = shader_->get()->SetValue(handle_, value.i, sizeof(value));
	if (FAILED(hr)) throw exception::directx_error(hr);
}

void ShaderParameter<math::matrix<4,4> >::set(math::matrix<4,4> const &value)
{
	HRESULT hr = shader_->get()->SetMatrix(handle_, (D3DXMATRIX const *) value.ij);
	if (FAILED(hr)) throw exception::directx_error(hr);
}

void ShaderParameter<Texture *>::set(Texture * const &texture)
{
	HRESULT hr = shader_->get()->SetTexture(handle_, texture->get());
	if (FAILED(hr)) throw exception::directx_error(hr);
}

namespace shader_parameter
{

typedef ShaderParameter<int> integer;
typedef ShaderParameter<math::vec<3> > float3;
typedef ShaderParameter<math::matrix<4,4> > float4x4;
typedef ShaderParameter<Texture *> texture;

}

}
