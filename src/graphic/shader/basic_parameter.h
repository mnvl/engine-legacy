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

namespace graphic
{
namespace shader
{

class BasicParameter
{
public:
	BasicParameter(std::string const &name):
		shader_(0),
		name_(name),
		handle_(0)
	{
	}

	~BasicParameter()
	{
	}

	void postconstruct(Shader *shader)
	{
		shader_ = shader;

		handle_ = shader_->get()->GetParameterByName(0, name_.c_str());
		if (!handle_) throw std::logic_error("can't get " + name_ + " parameter handle");
	}

private:
	Shader *shader_;
	std::string const name_;
	D3DXHANDLE handle_;
};

}
}
