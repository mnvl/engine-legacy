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

D3DVERTEXELEMENT9 *vertex_traits<VERTEX_PT>::get_descr()
{
	static D3DVERTEXELEMENT9 descr[] = {
		{
			0,
			offsetof(VERTEX_PT, position),
			D3DDECLTYPE_FLOAT3,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_POSITION,
			0
		},

		{
			0,
			offsetof(VERTEX_PT, texcoords),
			D3DDECLTYPE_FLOAT2,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD,
			0
		},

		D3DDECL_END()
	};

	return descr;
}

D3DVERTEXELEMENT9 *vertex_traits<VERTEX_PNT>::get_descr()
{
	static D3DVERTEXELEMENT9 descr[] = {
		{
			0,
			offsetof(VERTEX_PNT, position),
			D3DDECLTYPE_FLOAT3,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_POSITION,
			0
		},

		{
			0,
			offsetof(VERTEX_PNT, normal),
			D3DDECLTYPE_FLOAT3,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_NORMAL,
			0
		},

		{
			0,
			offsetof(VERTEX_PNT, texcoords),
			D3DDECLTYPE_FLOAT2,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD,
			0
		},

		D3DDECL_END()
	};

	return descr;
}

D3DVERTEXELEMENT9 *vertex_traits<VERTEX_PTBNT>::get_descr()
{
	static D3DVERTEXELEMENT9 descr[] = {
		{
			0,
			offsetof(VERTEX_PTBNT, position),
			D3DDECLTYPE_FLOAT3,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_POSITION,
			0
		},

		{
			0,
			offsetof(VERTEX_PTBNT, tangent),
			D3DDECLTYPE_FLOAT3,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TANGENT,
			0
		},

		{
			0,
			offsetof(VERTEX_PTBNT, normal),
			D3DDECLTYPE_FLOAT3,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_NORMAL,
			0
		},


		{
			0,
			offsetof(VERTEX_PTBNT, binormal),
			D3DDECLTYPE_FLOAT3,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_BINORMAL,
			0
		},

		{
			0,
			offsetof(VERTEX_PTBNT, texcoords),
			D3DDECLTYPE_FLOAT2,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD,
			0
		},

		D3DDECL_END()
	};

	return descr;
}

D3DVERTEXELEMENT9 *vertex_traits<VERTEX_PNJWT>::get_descr()
{
	static D3DVERTEXELEMENT9 descr[] = {
		{
			0,
			offsetof(VERTEX_PNJWT, position),
			D3DDECLTYPE_FLOAT3,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_POSITION,
			0
		},

		{
			0,
			offsetof(VERTEX_PNJWT, normal),
			D3DDECLTYPE_FLOAT3,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_NORMAL,
			0
		},

		{
			0,
			offsetof(VERTEX_PNJWT, joints),
			D3DDECLTYPE_UBYTE4,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_BLENDINDICES,
			0
		},

        {
			0,
			offsetof(VERTEX_PNJWT, joints) + 4,
			D3DDECLTYPE_UBYTE4,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_BLENDINDICES,
			1
		},

		{
			0,
			offsetof(VERTEX_PNJWT, weights),
			D3DDECLTYPE_FLOAT4,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_BLENDWEIGHT,
			0
		},

        {
			0,
			offsetof(VERTEX_PNJWT, weights) + 16,
			D3DDECLTYPE_FLOAT4,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_BLENDWEIGHT,
			1
		},

		{
			0,
			offsetof(VERTEX_PNJWT, texcoords),
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
		class_<VERTEX_PT>("vertex_pt")
		.def(constructor<>())
		.def_readwrite("position", &VERTEX_PT::position)
		.def_readwrite("texcoords", &VERTEX_PT::texcoords),

		class_<VERTEX_PNT>("vertex_pnt")
		.def(constructor<>())
		.def_readwrite("position", &VERTEX_PNT::position)
		.def_readwrite("normal", &VERTEX_PNT::normal)
		.def_readwrite("texcoords", &VERTEX_PNT::texcoords),

		class_<VERTEX_PNJWT>("vertex_pnjwt")
		.def(constructor<>())
		.def_readwrite("position", &VERTEX_PNJWT::position)
		.def_readwrite("normal", &VERTEX_PNJWT::normal)
		.def_readwrite("joints", &VERTEX_PNJWT::normal)
		.def_readwrite("wegiths", &VERTEX_PNJWT::normal)
		.def_readwrite("texcoords", &VERTEX_PNJWT::texcoords)
	 ];
}

}
