//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#pragma once

#include <boost/shared_ptr.hpp>
#include <lua.h>
#include <graphic/model.h>
#include <graphic/memory_model.h>
#include <graphic/static_mesh.h>
#include <graphic/renderer.h>

namespace graphic {

class StaticModel: public Model {
public:
	StaticModel(boost::shared_ptr<Renderer> const &renderer_ptr = boost::shared_ptr<Renderer>());
	~StaticModel();

	MeshPtr add_mesh();

    static void bind(lua_State *L);
};

}

