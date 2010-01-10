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

#include <graphic/static_mesh.h>

namespace graphic
{

class AnimatedModel;

class AnimatedMesh: public PnjwtStaticMesh {
public:
    AnimatedMesh(Model *model_ptr);

	virtual MeshPtr shallow_copy() const;

    AnimatedModel *get_model();
    virtual void draw(Joint *joint_ptr);

private:
	boost::shared_ptr<Shader> shader_;
};

}
