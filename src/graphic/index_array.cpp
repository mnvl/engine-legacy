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
		class_<IndexArray16, boost::shared_ptr<IndexArray16> >("IndexArray16")
		.def("lock", (INDEX_U16 *(IndexArray16::*)(bool)) &IndexArray16::lock)
		.def("unlock", (void (IndexArray16::*)()) &IndexArray16::unlock)
		.def("at", (INDEX_U16 *(IndexArray16::*)(size_t)) &IndexArray16::at)
		.def("get_index_buffer", &IndexArray16::get_index_buffer)
	 ];
}

}
