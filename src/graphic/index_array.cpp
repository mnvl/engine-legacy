//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <luabind/luabind.hpp>
#include <exception/directx_error.h>
#include "renderer.h"
#include "index_array.h"

namespace graphic
{

void bind_index_array(lua_State *L)
{
	using namespace luabind;

	module_(L, "graphic")
	[
		class_<IndexArray16, IndexBuffer, boost::shared_ptr<IndexArray16> >("IndexArray16")
		.def("lock", (index_t *(IndexArray16::*)(bool)) &IndexArray16::lock)
		.def("unlock", (void (IndexArray16::*)()) &IndexArray16::unlock)
		.def("at", (index_t *(IndexArray16::*)(size_t)) &IndexArray16::at)
	 ];
}

}
