#pragma once
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
#include "particle_system.h"

namespace graphic
{
namespace ps
{

template<class E, template<class> class C, template<class> class K, class D>
void ParticleSystem<E, C, K, D>::bind(char const *class_name, lua_State *L)
{
	using namespace luabind;

	module_(L, "ps")
	[
		class_<ParticleSystem, boost::shared_ptr<ParticleSystem> >(class_name)
		.def(constructor<boost::shared_ptr<Renderer> const &, size_t const>())
		.def("get_emitter", &ParticleSystem::get_emitter)
		.def("get_drawer", &ParticleSystem::get_drawer)
		.def("move", &ParticleSystem::move)
		.def("draw", &ParticleSystem::draw)
	];
}

}
}
