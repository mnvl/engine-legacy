//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <graphic/animated_mesh.h>
#include <graphic/animated_model.h>

namespace graphic
{

AnimatedModel::AnimatedModel(boost::shared_ptr<Renderer> const &renderer_ptr):
	Model(renderer_ptr)
{
}

AnimatedModel::~AnimatedModel()
{
}

MeshPtr AnimatedModel::add_mesh()
{
	meshes.push_back(MeshPtr(new AnimatedMesh(this)));
	return meshes.back();
}

}
