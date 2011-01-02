//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2011 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include "renderer.h"
#include "mesh_draw_call.h"

namespace graphic
{

MeshDrawCall::MeshDrawCall(RendererPtr const &renderer):
	DrawCall(renderer),
	shader_(renderer->get_uber_shader())
{
}

MeshDrawCall::~MeshDrawCall()
{
}

void MeshDrawCall::swap_joint_matrix_array(std::vector<math::matrix<4,4> > &joint_matrix_array)
{
	joint_matrix_array_.swap(joint_matrix_array);
}

void MeshDrawCall::set_joint_matrix_array(std::vector<math::matrix<4,4> > &joint_matrix_array)
{
	joint_matrix_array_ = joint_matrix_array;
}

}
