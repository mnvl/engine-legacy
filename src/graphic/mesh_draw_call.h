#pragma once
//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2011 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include "uber_shader.h"
#include "draw_call.h"

namespace graphic
{

class MeshDrawCall: public DrawCall {
public:
	MeshDrawCall(RendererPtr const &renderer);
	virtual ~MeshDrawCall();

	void swap_joint_matrix_array(std::vector<math::matrix<4,4> > &joint_matrix_array);
	void set_joint_matrix_array(std::vector<math::matrix<4,4> > &joint_matrix_array);

protected:
	UberShaderPtr const shader_;
	std::vector<math::matrix<4,4> > joint_matrix_array_;
};

}
