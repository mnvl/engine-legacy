//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2011 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include "animated_mesh.h"
#include "animated_model.h"

namespace graphic
{

template<class V>
BasicAnimatedModel<V>::BasicAnimatedModel(RendererPtr const &renderer_ptr, SchedulerPtr const &scheduler_ptr):
	Model(renderer_ptr, scheduler_ptr)
{
}

template<class V>
BasicAnimatedModel<V>::~BasicAnimatedModel()
{
}

template<class V>
MeshPtr BasicAnimatedModel<V>::add_mesh()
{
	meshes.push_back(MeshPtr(new BasicAnimatedMesh<V>(this)));
	return meshes.back();
}

template AnimatedModelPNJWT;
template AnimatedModelPTBNJWT;

}
