//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <graphic/model.h>
#include <graphic/mesh.h>

namespace graphic {

Mesh::Mesh(Model *model_ptr):
	model_ptr_(model_ptr),
	shader_(model_ptr->get_renderer()->get_shared_resources()->get_mesh_shader())
{
}

Mesh::~Mesh()
{
}

void Mesh::set_model(Model *model)
{
	model_ptr_ = model;
}

Model *Mesh::get_model() const
{
	return model_ptr_;
}

void Mesh::draw(Joint *joint_ptr)
{
}

}


