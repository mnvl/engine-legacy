//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include "resource.h"
#include "vertex.h"
#include "vertex_buffer.h"
#include "vertex_array.h"
#include "index_buffer.h"
#include "index_array.h"
#include "material.h"
#include "texture.h"
#include "draw_call.h"
#include "joint.h"
#include "mesh.h"
#include "memory_mesh.h"
#include "model.h"
#include "static_model.h"
#include "animated_model.h"
#include "memory_model.h"
#include "collada_loader.h"
#include "camera.h"
#include "font.h"
#include "renderer.h"
#include "scheduler.h"
#include "bind.h"

namespace graphic
{

void bind(lua_State *L)
{
	Resource::bind(L);
	bind_vertex(L);
	VertexBuffer::bind(L);
	bind_vertex_array(L);
	IndexBuffer::bind(L);
	bind_index_array(L);
	Material::bind(L);
	Texture::bind(L);
	DrawCall::bind(L);
	Joint::bind(L);
    Mesh::bind(L);
	MemoryMesh::bind(L);
    Model::bind(L);
    bind_static_model(L);
	bind_animated_model(L);
	MemoryModel::bind(L);
	ColladaLoader::bind(L);
	Camera::bind(L);
	Font::bind(L);
    Renderer::bind(L);
	Scheduler::bind(L);
}

}
