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

#include <boost/cstdint.hpp>

namespace rpc
{

#pragma pack(push, 1)
struct Header
{
	boost::uint16_t id;
	boost::uint16_t len;
};
#pragma pack(pop)

}
