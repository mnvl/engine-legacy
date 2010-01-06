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
#include <math/matrix.h>
#include "model.h"
#include "memory_mesh.h"

namespace graphic {

class MemoryModel: public Model {
public:
	MemoryModel(boost::shared_ptr<Renderer> const &renderer_ptr = boost::shared_ptr<Renderer>());

	MemoryMeshPtr get_mesh_by_id(std::string const &id) {
		return boost::static_pointer_cast<MemoryMesh>(Model::get_mesh_by_id(id));
	}

	MemoryMeshPtr get_mesh_by_name(std::string const &name) {
		return boost::static_pointer_cast<MemoryMesh>(Model::get_mesh_by_name(name));
	}

	virtual MeshPtr add_mesh();

	virtual void draw();
	void draw_static();
	void draw_animated();

	static void bind(lua_State *L);
};

}
