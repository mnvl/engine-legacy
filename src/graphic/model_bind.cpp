//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <boost/shared_ptr.hpp>
#include <luabind/luabind.hpp>
#include <luabind/iterator_policy.hpp>
#include "model.h"

void graphic::Model::bind(lua_State *L)
{
    using namespace luabind;

    module_(L, "graphic")
    [
        class_<Model, boost::shared_ptr<Model> >("Model")
		.def("load_from_collada", &Model::load_from_collada)
		.def("set_animation_time", &Model::set_animation_time)
		.def("draw", &Model::draw)
		.def("get_joint_by_id", &Model::get_joint_by_id)
		.def("get_joint_by_name", &Model::get_joint_by_name)
		.def_readonly("meshes", &Model::meshes, return_stl_iterator)
    ];
}

