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

#include <string>
#include <d3d9.h>
#include <d3dx9.h>
#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <util/boost_com_intrusive_ptr.h>
#include <math/vec.h>
#include <math/matrix.h>
#include "texture.h"

namespace graphic
{

class Shader;
class Renderer;

class Shader {
public:
    Shader(boost::shared_ptr<Renderer> const &renderer_ptr);
	virtual ~Shader();

    void load_from_file(std::string const &filename);
    void load_from_buffer(char const *buffer, size_t len);

    ID3DXEffect *get() const
    {
        return effect_ptr_.get();
    }

private:
	boost::shared_ptr<Renderer> const renderer_ptr_;
    boost::intrusive_ptr<ID3DXEffect> effect_ptr_;

	static void log_compilation_errors(ID3DXBuffer *compilation_errors_ptr);
};

namespace shader_parameters
{

static size_t const MAX_POINT_LIGHTS = 5;

#pragma pack(push, 1)
struct POINT_LIGHT
{
	math::vec<3> position;
	math::vec<4> color;
	math::scalar inv_attenuation;
};

struct POINT_LIGHTS
{
	POINT_LIGHT items[MAX_POINT_LIGHTS];
};
#pragma pack(pop)

class BasicParameter
{
public:
	BasicParameter(Shader *shader, std::string const &name):
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

protected:
	Shader *shader_;
	std::string const name_;
	D3DXHANDLE handle_;
};

template<class T>
class Parameter: public BasicParameter
{
public:
	Parameter(Shader *shader, std::string const &name):
		BasicParameter(shader, name)
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

class Technique
{
public:
	Technique(Shader *shader, std::string const &name):
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

protected:
	Shader *shader_;
	std::string const name_;
	D3DXHANDLE handle_;
};

typedef Parameter<math::vec<3> > float3;
typedef Parameter<math::matrix<4, 4> > float4x4;
typedef Parameter<boost::shared_ptr<Texture> > texture;
typedef Parameter<POINT_LIGHTS> point_lights;

}

}
