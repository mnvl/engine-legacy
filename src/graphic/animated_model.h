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

#include "model.h"

namespace graphic
{

class Mesh;
class Renderer;

class AnimatedModel: public Model {
public:
	AnimatedModel(boost::shared_ptr<Renderer> const &renderer_ptr, boost::shared_ptr<Scheduler> const &scheduler_ptr);
	~AnimatedModel();

	virtual MeshPtr add_mesh();

    static void bind(lua_State *L);
};


}
