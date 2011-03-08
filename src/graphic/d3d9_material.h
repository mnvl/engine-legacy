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

#include "material.h"

namespace graphic
{

class D3D9Renderer;

class D3D9Material: public Material {
	friend class D3D9Renderer;

public:
	virtual ~D3D9Material();

	virtual void select();

private:
	D3D9RendererPtr renderer_;

	D3D9Material(D3D9RendererPtr const &renderer_ptr = D3D9RendererPtr());
};

}
