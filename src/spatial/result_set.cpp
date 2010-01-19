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
#include <luabind/iterator_policy.hpp>
#include "result_set.h"

namespace spatial
{

void ResultSet::bind(lua_State *L)
{
	using namespace luabind;

	module(L, "spatial")
	[
		class_<ResultSet, ResultSetPtr>("ResultSet")
		.def(constructor<>())
		.def_readonly("shapes", &ResultSet::shapes_, return_stl_iterator)
	];
}

}
