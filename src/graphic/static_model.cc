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
#include <exception/directx_error.h>
#include "static_model.h"

namespace graphic
{

template<class V>
BasicStaticModel<V>::BasicStaticModel(RendererPtr const &renderer_ptr, SchedulerPtr const &scheduler_ptr):
	Model(renderer_ptr, scheduler_ptr)
{
}

template<class V>
BasicStaticModel<V>::~BasicStaticModel()
{
}

template<class V>
MeshPtr BasicStaticModel<V>::add_mesh()
{
	meshes.push_back(MeshPtr(new BasicStaticMesh<V>(this)));
	return meshes.back();
}

template<class V>
void BasicStaticModel<V>::bind(lua_State *L, char const *classname)
{
    using namespace luabind;

    module_(L, "graphic")
    [
        class_<BasicStaticModel, Model, boost::shared_ptr<BasicStaticModel> >(classname)
		.def(constructor<BasicStaticModel const &>())
        .def(constructor<RendererPtr const &, SchedulerPtr const &>())
    ];
}

void bind_static_model(lua_State *L)
{
	StaticModelPNT::bind(L, "StaticModelPNT");
	StaticModelPTBNT::bind(L, "StaticModelPTBNT");
}

template StaticModelPNT;
template StaticModelPTBNT;

}
