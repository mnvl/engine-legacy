//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2012 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <luabind/luabind.hpp>
#include "contact_info.h"

namespace math
{

template<int N, class T>
void contact_info<N, T>::bind(lua_State *L, char const *name)
{
	using namespace luabind;

	module(L, "math")
	[
		class_<contact_info>(name)
		.def(constructor<>())
		.def_readwrite("happened", &contact_info::happened)
		.def_readwrite("penetrated", &contact_info::penetrated)
		.def_readwrite("position", &contact_info::position)
		.def_readwrite("normal", &contact_info::normal)
		.def_readwrite("time", &contact_info::time)
		.def_readwrite("penetration_depth", &contact_info::penetration_depth)
	];
}

void bind_contact_info(lua_State *L)
{
	contact_info<2>::bind(L, "contact_info_2");
	contact_info<3>::bind(L, "contact_info_3");
}

}

