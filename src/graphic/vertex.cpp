//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <stddef.h>
#include <d3dx9.h>
#include <luabind/luabind.hpp>
#include <graphic/vertex.h>

namespace graphic
{

D3DVERTEXELEMENT9 *vertex_traits<vertex_pt>::get_descr()
{
	static D3DVERTEXELEMENT9 descr[] = {
		{
			0,
			offsetof(vertex_pt, position),
			D3DDECLTYPE_FLOAT3,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_POSITION,
			0
		},

		{
			0,
			offsetof(vertex_pt, texcoords),
			D3DDECLTYPE_FLOAT2,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD,
			0
		},

		D3DDECL_END()
	};

	return descr;
}

D3DVERTEXELEMENT9 *vertex_traits<vertex_pnt>::get_descr()
{
	static D3DVERTEXELEMENT9 descr[] = {
		{
			0,
			offsetof(vertex_pnt, position),
			D3DDECLTYPE_FLOAT3,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_POSITION,
			0
		},

		{
			0,
			offsetof(vertex_pnt, normal),
			D3DDECLTYPE_FLOAT3,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_NORMAL,
			0
		},

		{
			0,
			offsetof(vertex_pnt, texcoords),
			D3DDECLTYPE_FLOAT2,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD,
			0
		},

		D3DDECL_END()
	};

	return descr;
}

D3DVERTEXELEMENT9 *vertex_traits<vertex_pnjwt>::get_descr()
{
	static D3DVERTEXELEMENT9 descr[] = {
		{
			0,
			offsetof(vertex_pnjwt, position),
			D3DDECLTYPE_FLOAT3,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_POSITION,
			0
		},

		{
			0,
			offsetof(vertex_pnjwt, normal),
			D3DDECLTYPE_FLOAT3,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_NORMAL,
			0
		},

		{
			0,
			offsetof(vertex_pnjwt, joints),
			D3DDECLTYPE_UBYTE4,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_BLENDINDICES,
			0
		},

        {
			0,
			offsetof(vertex_pnjwt, joints) + 4,
			D3DDECLTYPE_UBYTE4,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_BLENDINDICES,
			1
		},

		{
			0,
			offsetof(vertex_pnjwt, weights),
			D3DDECLTYPE_FLOAT4,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_BLENDWEIGHT,
			0
		},

        {
			0,
			offsetof(vertex_pnjwt, weights) + 16,
			D3DDECLTYPE_FLOAT4,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_BLENDWEIGHT,
			1
		},

		{
			0,
			offsetof(vertex_pnjwt, texcoords),
			D3DDECLTYPE_FLOAT2,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD,
			0
		},

		D3DDECL_END()
	};

	return descr;
}

void bind_vertex(lua_State *L)
{
	using namespace luabind;

	module(L, "graphic")
	[
		class_<vertex_pt>("vertex_pt")
		.def(constructor<>())
		.def_readwrite("position", &vertex_pt::position)
		.def_readwrite("texcoords", &vertex_pt::texcoords),

		class_<vertex_pnt>("vertex_pnt")
		.def(constructor<>())
		.def_readwrite("position", &vertex_pnt::position)
		.def_readwrite("normal", &vertex_pnt::normal)
		.def_readwrite("texcoords", &vertex_pnt::texcoords),

		class_<vertex_pnjwt>("vertex_pnjwt")
		.def(constructor<>())
		.def_readwrite("position", &vertex_pnjwt::position)
		.def_readwrite("normal", &vertex_pnjwt::normal)
		.def_readwrite("joints", &vertex_pnjwt::normal)
		.def_readwrite("wegiths", &vertex_pnjwt::normal)
		.def_readwrite("texcoords", &vertex_pnjwt::texcoords)
	 ];
}

}
