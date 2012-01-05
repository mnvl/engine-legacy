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

#include "draw_call.h"

namespace graphic
{

class D3D9DrawCall: public DrawCall {
public:
	D3D9DrawCall(D3D9RendererPtr const &renderer);
	virtual ~D3D9DrawCall();

	virtual void execute();

private:
	D3D9RendererPtr renderer_;
};

}
