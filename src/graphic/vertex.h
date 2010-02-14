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

#include <d3d9.h>
#include <boost/cstdint.hpp>
#include <math/vec.h>

namespace graphic
{

#pragma pack(push, 1)
struct vertex_pt
{
	math::vec<3> position;
	math::vec<2> texcoords;
};

struct vertex_pnt
{
	math::vec<3> position;
	math::vec<3> normal;
	math::vec<2> texcoords;
};

struct vertex_ptbnt
{
	math::vec<3> position;
	math::vec<3> tangent;
	math::vec<3> binormal;
	math::vec<3> normal;
	math::vec<2> texcoords;
};

struct vertex_pnjwt
{
	math::vec<3> position;
	math::vec<3> normal;
	math::vec<8, boost::uint8_t> joints;
	math::vec<8> weights;
	math::vec<2> texcoords;
};
#pragma pack(pop)

template <class T>
struct vertex_traits;

template<>
struct vertex_traits<vertex_pt>
{
	static D3DVERTEXELEMENT9 *get_descr();
};

template<>
struct vertex_traits<vertex_pnt>
{
	static D3DVERTEXELEMENT9 *get_descr();
};

template<>
struct vertex_traits<vertex_ptbnt>
{
	static D3DVERTEXELEMENT9 *get_descr();
};

template<>
struct vertex_traits<vertex_pnjwt>
{
	static D3DVERTEXELEMENT9 *get_descr();
};

void bind_vertex(lua_State *L);

}
