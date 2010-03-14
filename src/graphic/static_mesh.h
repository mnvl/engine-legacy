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

#include <d3d9.h>
#include <util/boost_com_intrusive_ptr.h>
#include <math/aabb.h>
#include "vertex.h"
#include "vertex_array.h"
#include "index.h"
#include "index_array.h"
#include "mesh.h"

namespace graphic
{

template<class T> class BasicStaticModel;

template<class V = VERTEX_PNT>
class BasicStaticMesh: public Mesh {
public:
	typedef V vertex_t;

	BasicStaticMesh(Model *model_ptr);
	~BasicStaticMesh();

	virtual MeshPtr shallow_copy() const;

	BasicStaticModel<V> *get_model();
	virtual void draw(Joint *joint_ptr);

	virtual void set_vertices(GenericVertex *vertices, GenericVertex::format_t format, size_t count);
	virtual void set_indices(unsigned *indices, size_t count);

protected:
	typedef VertexArray<V> vertex_buffer_t;
	typedef IndexArray16 index_buffer_t;

	// AABB over positions of all vertices before applyed transform
	math::aabb<3> bounds_;

	GenericVertex::format_t initialized_vertex_components_;
	boost::shared_ptr<vertex_buffer_t> vertex_buffer_;
	boost::shared_ptr<index_buffer_t> index_buffer_;
};

typedef BasicStaticMesh<VERTEX_PNT> StaticMeshPNT;
typedef BasicStaticMesh<VERTEX_PTBNT> StaticMeshPTBNT;
typedef BasicStaticMesh<VERTEX_PNJWT> StaticMeshPNJWT;
typedef BasicStaticMesh<VERTEX_PTBNJWT> StaticMeshPTBNJWT;

}
