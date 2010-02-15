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
#include <exception/directx_error.h>
#include "shader.h"

namespace graphic
{
namespace shader
{

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

}
}
