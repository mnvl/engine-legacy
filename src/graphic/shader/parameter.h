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
#include <graphic/texture.h>
#include "basic_parameter.h"
#include "shader.h"

namespace graphic
{
namespace shader
{

template<class T>
class Parameter: public BasicParameter
{
public:
	Parameter(std::string const &name):
		BasicParameter(name)
	{
	}

	inline void set(T const &value)
	{
		HRESULT hr = shader_->get()->SetValue(handle_, &value, sizeof(value));
		if (FAILED(hr)) throw exception::directx_error(hr);
	}
};

void Parameter<math::matrix<4,4> >::set(math::matrix<4,4> const &value)
{
	HRESULT hr = shader_->get()->SetMatrix(handle_, (D3DXMATRIX const *) value.ij);
	if (FAILED(hr)) throw exception::directx_error(hr);
}

void Parameter<boost::shared_ptr<Texture> >::set(boost::shared_ptr<Texture> const &texture)
{
	HRESULT hr = shader_->get()->SetTexture(handle_, texture->get());
	if (FAILED(hr)) throw exception::directx_error(hr);
}

namespace parameter
{

typedef Parameter<int> integer;
typedef Parameter<math::vec<3> > float3;
typedef Parameter<math::matrix<4,4> > float4x4;
typedef Parameter<boost::shared_ptr<Texture> > texture;

}

}
}
