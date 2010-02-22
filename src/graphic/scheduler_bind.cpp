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
#include "scheduler.h"

namespace graphic
{

void Scheduler::bind(lua_State *L)
{
	using namespace luabind;

	module_(L, "graphic")
	[
		class_<Scheduler, boost::shared_ptr<Scheduler> >("Scheduler")
		.def(constructor<boost::shared_ptr<Renderer> const &>())
		.def("add", &Scheduler::add)
		.def("flush", &Scheduler::flush)
	];
}

}
