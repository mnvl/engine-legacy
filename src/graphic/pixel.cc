//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2012 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <stdexcept>
#include <boost/lexical_cast.hpp>
#include "pixel.h"

namespace graphic
{

static PixelFormat pixel_format_X8R8G8B8 = {
	math::vec<4, unsigned>(0x0000FF0000u, 0x0000FF00u, 0x000000FFu, 0xFF000000u),
	D3DFMT_X8R8G8B8,
	32
};

static PixelFormat pixel_format_A8R8G8B8 = {
	math::vec<4, unsigned>(0x0000FF0000u, 0x0000FF00u, 0x000000FFu, 0xFF000000u),
	D3DFMT_A8R8G8B8,
	32
};

PixelFormat const &PixelFormat::get(unsigned pixel_format) {
	switch (pixel_format) {
	case X8R8G8B8:
		return pixel_format_X8R8G8B8;

	case A8R8G8B8:
		return pixel_format_A8R8G8B8;

	default:
		throw std::runtime_error("unknown pixel format " +
			boost::lexical_cast<std::string>(pixel_format));
	}
}

}
