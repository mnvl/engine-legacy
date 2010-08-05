//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include "renderer.h"
#include "memory_model.h"

namespace graphic
{

MemoryModel::MemoryModel(boost::shared_ptr<Renderer> const &renderer_ptr, boost::shared_ptr<Scheduler> const &scheduler_ptr):
	Model(renderer_ptr, scheduler_ptr)
{
}

MemoryModel::~MemoryModel()
{
}

MeshPtr MemoryModel::add_mesh()
{
	meshes.push_back(MeshPtr(new MemoryMesh(this)));
	return meshes.back();
}

void MemoryModel::draw(boost::shared_ptr<Camera> const &camera, math::matrix<4,4> const &world_matrix)
{
	if (animations.size()) draw_animated(camera, world_matrix);
	else draw_static(camera, world_matrix);
}

void MemoryModel::draw_static(boost::shared_ptr<Camera> const &camera, math::matrix<4,4> const &world_matrix)
{
	set_animation_time(0);

	for (joints_type::iterator it = joints.begin(); it != joints.end(); it++)
	{
		if (MemoryMeshPtr mesh_ptr = boost::static_pointer_cast<MemoryMesh>((*it)->mesh_ptr.lock()))
		{
			mesh_ptr->draw_static(it->get(), camera, world_matrix);
		}
	}
}

void MemoryModel::draw_animated(boost::shared_ptr<Camera> const &camera, math::matrix<4,4> const &world_matrix)
{
	for (joints_type::iterator it = joints.begin(); it != joints.end(); it++)
	{
		if (MemoryMeshPtr mesh_ptr = boost::static_pointer_cast<MemoryMesh>((*it)->mesh_ptr.lock()))
		{
			mesh_ptr->draw_animated(it->get(), camera, world_matrix);
		}
	}
}

}
