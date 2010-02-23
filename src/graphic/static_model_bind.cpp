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
#include "static_model.h"

void graphic::StaticModel::bind(lua_State *L)
{
    using namespace luabind;

    module_(L, "graphic")
    [
        class_<StaticModel, Model, boost::shared_ptr<StaticModel> >("StaticModel")
		.def(constructor<StaticModel const &>())
        .def(constructor<boost::shared_ptr<Renderer> const &, boost::shared_ptr<Scheduler> const &>())
    ];
}
