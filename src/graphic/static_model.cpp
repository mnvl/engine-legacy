//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <boost/lambda/lambda.hpp>
#include <exception/directx_error.h>
#include "static_model.h"

namespace graphic
{

StaticModel::StaticModel(boost::shared_ptr<Renderer> const &renderer_ptr, boost::shared_ptr<Scheduler> const &scheduler_ptr):
	Model(renderer_ptr, scheduler_ptr)
{
}

StaticModel::~StaticModel()
{
}

MeshPtr StaticModel::add_mesh()
{
	meshes.push_back(MeshPtr(new StaticMesh(this)));
	return meshes.back();
}

}
