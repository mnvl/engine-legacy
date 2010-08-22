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

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "forward.h"

namespace graphic
{

class MeshShader;

class ShadersLibrary {
public:
    ShadersLibrary(Renderer * const renderer_ptr);
    ~ShadersLibrary();

	boost::shared_ptr<MeshShader> get_mesh_shader();

private:
	Renderer * const renderer_;

	boost::weak_ptr<MeshShader> mesh_shader_;
};

}
