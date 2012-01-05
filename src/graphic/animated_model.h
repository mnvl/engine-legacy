#pragma once
//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2012 Manvel Avetisian.
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

template<class V = VERTEX_PNJWT>
class BasicAnimatedModel: public Model {
public:
	BasicAnimatedModel(RendererPtr const &renderer_ptr, SchedulerPtr const &scheduler_ptr);
	~BasicAnimatedModel();

	virtual MeshPtr add_mesh();

    static inline void bind(lua_State *L, char const *classname);
};

typedef BasicAnimatedModel<VERTEX_PNJWT> AnimatedModelPNJWT;
typedef BasicAnimatedModel<VERTEX_PTBNJWT> AnimatedModelPTBNJWT;

void bind_animated_model(lua_State *L);

}
