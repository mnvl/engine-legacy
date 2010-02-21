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

#include "mesh_shader.h"
#include "draw_call.h"

namespace graphic
{

class MeshDrawCall: public DrawCall
{
public:
	MeshDrawCall(boost::shared_ptr<Renderer> const &renderer, boost::shared_ptr<MeshShader> shader);
	~MeshDrawCall();

	virtual void execute();

private:
	boost::shared_ptr<MeshShader> shader_;
};

}
