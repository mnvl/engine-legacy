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

#include <d3d9.h>
#include <math/vec.h>

namespace graphic
{

struct PixelFormat {
	static unsigned const X8R8G8B8 = 1;
	static unsigned const A8R8G8B8 = 2;

	static unsigned const DEFAULT_FORMAT = X8R8G8B8;
	static unsigned const DEFAULT_ALPHA_FORMAT = A8R8G8B8;


	const math::vec<4, unsigned> masks;
	const D3DFORMAT direct3d_format;
	const int bits;

	static PixelFormat const &get(unsigned pixel_format);
};

}
