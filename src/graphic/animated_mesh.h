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

#include "static_mesh.h"

namespace graphic
{

class MeshShader;
template<class V> class BasicAnimatedModel;

template<class V = VERTEX_PNJWT>
class BasicAnimatedMesh: public BasicStaticMesh<V> {
public:
    BasicAnimatedMesh(Model *model_ptr);
	~BasicAnimatedMesh();

	virtual MeshPtr shallow_copy() const;

    BasicAnimatedModel<V> *get_model();

	virtual void check();

    virtual void draw(Joint *joint_ptr, CameraPtr const &camera, math::matrix<4,4> const &world_matrix);
};

typedef BasicAnimatedMesh<VERTEX_PNJWT> AnimatedMeshPNJWT;
typedef BasicAnimatedMesh<VERTEX_PTBNJWT> AnimatedMeshPTBNJWT;

}
