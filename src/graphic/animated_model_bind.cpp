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
#include "animated_model.h"

namespace graphic
{

template<class V>
void BasicAnimatedModel<V>::bind(lua_State *L, char const *classname)
{
    using namespace luabind;

    module_(L, "graphic")
    [
        class_<BasicAnimatedModel, Model, boost::shared_ptr<BasicAnimatedModel> >(classname)
		.def(constructor<BasicAnimatedModel const &>())
        .def(constructor<RendererPtr const &, SchedulerPtr const &>())
	];
}

void bind_animated_model(lua_State *L)
{
	AnimatedModelPNJWT::bind(L, "AnimatedModelPNJWT");
	AnimatedModelPTBNJWT::bind(L, "AnimatedModelPTBNJWT");
}

}
