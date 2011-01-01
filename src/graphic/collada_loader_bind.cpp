//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2011 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <boost/shared_ptr.hpp>
#include <luabind/luabind.hpp>
#include "collada_loader.h"

namespace graphic
{

void ColladaLoader::bind(lua_State *L)
{
    using namespace luabind;

    module_(L, "graphic")
    [
        class_<ColladaLoader, boost::shared_ptr<ColladaLoader> >("ColladaLoader")
        .def(constructor<Model *>())
		.def("load", &ColladaLoader::load)
		.def("build_model", &ColladaLoader::build_model)
    ];
}

}
