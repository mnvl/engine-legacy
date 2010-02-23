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
#include "vertex_declarations.h"
#include "vertex_array.h"

namespace graphic
{

template<class T>
VertexTraits const *VertexArray<T>::get_vertex_traits() const
{
	return VertexTraits::get<T>();
}

template<class T>
IDirect3DVertexDeclaration9 *VertexArray<T>::get_vertex_decl() const
{
	return renderer_->get_vertex_declarations()->get<T>();
}

template class VertexArray<VERTEX_PT>;
template class VertexArray<VERTEX_PNT>;
template class VertexArray<VERTEX_PTBNT>;
template class VertexArray<VERTEX_PNJWT>;
template class VertexArray<VERTEX_PTBNJWT>;

void bind_vertex_array(lua_State *L)
{
	using namespace luabind;

	module_(L, "graphic")
	[
		class_<VertexArrayPT, VertexBuffer, boost::shared_ptr<VertexArrayPT> >("VertexArrayPT")
		.def("lock", (VERTEX_PT *(VertexArrayPT::*)(bool)) &VertexArrayPT::lock)
		.def("unlock", (void (VertexArrayPT::*)()) &VertexArrayPT::unlock)
		.def("at", (VERTEX_PT *(VertexArrayPT::*)(size_t)) &VertexArrayPT::at),

		class_<VertexArrayPNT, VertexBuffer, boost::shared_ptr<VertexArrayPNT> >("VertexArrayPNT")
		.def("lock", (VERTEX_PNT *(VertexArrayPNT::*)(bool)) &VertexArrayPNT::lock)
		.def("unlock", (void (VertexArrayPNT::*)()) &VertexArrayPNT::unlock)
		.def("at", (VERTEX_PNT *(VertexArrayPNT::*)(size_t)) &VertexArrayPNT::at),

		class_<VertexArrayPNJWT, VertexBuffer, boost::shared_ptr<VertexArrayPNJWT> >("VertexArrayPNJWT")
		.def("lock", (VERTEX_PNJWT *(VertexArrayPNJWT::*)(bool)) &VertexArrayPNJWT::lock)
		.def("unlock", (void (VertexArrayPNJWT::*)()) &VertexArrayPNJWT::unlock)
		.def("at", (VERTEX_PNJWT *(VertexArrayPNJWT::*)(size_t)) &VertexArrayPNJWT::at),

		class_<VertexArrayPTBNJWT, VertexBuffer, boost::shared_ptr<VertexArrayPTBNJWT> >("VertexArrayPTBNJWT")
		.def("lock", (VERTEX_PTBNJWT *(VertexArrayPTBNJWT::*)(bool)) &VertexArrayPTBNJWT::lock)
		.def("unlock", (void (VertexArrayPTBNJWT::*)()) &VertexArrayPTBNJWT::unlock)
		.def("at", (VERTEX_PTBNJWT *(VertexArrayPTBNJWT::*)(size_t)) &VertexArrayPTBNJWT::at)
	];
}

}
