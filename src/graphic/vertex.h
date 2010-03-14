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

#include <boost/cstdint.hpp>
#include <math/vec.h>

#include "generic_vertex.h"

namespace graphic
{

#pragma pack(push, 1)
struct VERTEX_PT
{
	static GenericVertex::format_t const FORMAT = GenericVertex::FORMAT_POSITION | GenericVertex::FORMAT_TEXCOORDS;

	math::vec<3> position;
	math::vec<2> texcoords;

	VERTEX_PT &operator =(GenericVertex const &rhs)
	{
		position  = rhs.position;
		texcoords = rhs.texcoords;
		return *this;
	}

	void assign(GenericVertex const &v, GenericVertex::format_t format)
	{
		if (format & GenericVertex::FORMAT_POSITION) position = v.position;
		if (format & GenericVertex::FORMAT_TEXCOORDS) texcoords = v.texcoords;
	}
};

struct VERTEX_PNT
{
	static GenericVertex::format_t const FORMAT = GenericVertex::FORMAT_POSITION |
		GenericVertex::FORMAT_NORMAL |
		GenericVertex::FORMAT_TEXCOORDS;

	math::vec<3> position;
	math::vec<3> normal;
	math::vec<2> texcoords;

	VERTEX_PNT &operator =(GenericVertex const &rhs)
	{
		position  = rhs.position;
		normal    = rhs.normal;
		texcoords = rhs.texcoords;
		return *this;
	}

	void assign(GenericVertex const &v, GenericVertex::format_t format)
	{
		if (format & GenericVertex::FORMAT_POSITION) position = v.position;
		if (format & GenericVertex::FORMAT_NORMAL) normal = v.normal;
		if (format & GenericVertex::FORMAT_TEXCOORDS) texcoords = v.texcoords;
	}
};

struct VERTEX_PTBNT
{
	static GenericVertex::format_t const FORMAT = GenericVertex::FORMAT_POSITION |
		GenericVertex::FORMAT_TANGENT |
		GenericVertex::FORMAT_BINORMAL |
		GenericVertex::FORMAT_NORMAL |
		GenericVertex::FORMAT_TEXCOORDS;

	math::vec<3> position;
	math::vec<3> tangent;
	math::vec<3> binormal;
	math::vec<3> normal;
	math::vec<2> texcoords;

	VERTEX_PTBNT &operator =(GenericVertex const &rhs)
	{
		position  = rhs.position;
		tangent   = rhs.tangent;
		binormal  = rhs.binormal;
		normal    = rhs.normal;
		texcoords = rhs.texcoords;
		return *this;
	}

	void assign(GenericVertex const &v, GenericVertex::format_t format)
	{
		if (format & GenericVertex::FORMAT_POSITION) position = v.position;
		if (format & GenericVertex::FORMAT_TANGENT) tangent = v.tangent;
		if (format & GenericVertex::FORMAT_BINORMAL) binormal = v.binormal;
		if (format & GenericVertex::FORMAT_NORMAL) normal = v.normal;
		if (format & GenericVertex::FORMAT_TEXCOORDS) texcoords = v.texcoords;
	}
};

struct VERTEX_PNJWT
{
	static GenericVertex::format_t const FORMAT = GenericVertex::FORMAT_POSITION |
		GenericVertex::FORMAT_NORMAL |
		GenericVertex::FORMAT_JOINT_INDICES |
		GenericVertex::FORMAT_JOINT_WEIGHTS |
		GenericVertex::FORMAT_TEXCOORDS;

	math::vec<3> position;
	math::vec<3> normal;
	math::vec<8, boost::uint8_t> joints;
	math::vec<8> weights;
	math::vec<2> texcoords;

	VERTEX_PNJWT &operator =(GenericVertex const &rhs)
	{
		position  = rhs.position;
		normal    = rhs.normal;
		joints    = rhs.joints;
		weights   = rhs.weights;
		texcoords = rhs.texcoords;
		return *this;
	}

	void assign(GenericVertex const &v, GenericVertex::format_t format)
	{
		if (format & GenericVertex::FORMAT_POSITION) position = v.position;
		if (format & GenericVertex::FORMAT_NORMAL) normal = v.normal;
		if (format & GenericVertex::FORMAT_JOINT_INDICES) joints = v.joints;
		if (format & GenericVertex::FORMAT_JOINT_WEIGHTS) weights = v.weights;
		if (format & GenericVertex::FORMAT_TEXCOORDS) texcoords = v.texcoords;
	}
};

struct VERTEX_PTBNJWT
{
	static GenericVertex::format_t const FORMAT = GenericVertex::FORMAT_POSITION |
		GenericVertex::FORMAT_TANGENT |
		GenericVertex::FORMAT_BINORMAL |
		GenericVertex::FORMAT_NORMAL |
		GenericVertex::FORMAT_JOINT_INDICES |
		GenericVertex::FORMAT_JOINT_WEIGHTS |
		GenericVertex::FORMAT_TEXCOORDS;

	math::vec<3> position;
	math::vec<3> tangent;
	math::vec<3> binormal;
	math::vec<3> normal;
	math::vec<8, boost::uint8_t> joints;
	math::vec<8> weights;
	math::vec<2> texcoords;

	VERTEX_PTBNJWT &operator =(GenericVertex const &v)
	{
		position  = v.position;
		tangent   = v.tangent;
		binormal  = v.binormal;
		normal    = v.normal;
		joints    = v.joints;
		weights   = v.weights;
		texcoords = v.texcoords;
		return *this;
	}

	void assign(GenericVertex const &v, GenericVertex::format_t format)
	{
		if (format & GenericVertex::FORMAT_POSITION) position = v.position;
		if (format & GenericVertex::FORMAT_TANGENT) tangent = v.tangent;
		if (format & GenericVertex::FORMAT_BINORMAL) binormal = v.binormal;
		if (format & GenericVertex::FORMAT_NORMAL) normal = v.normal;
		if (format & GenericVertex::FORMAT_JOINT_INDICES) joints = v.joints;
		if (format & GenericVertex::FORMAT_JOINT_WEIGHTS) weights = v.weights;
		if (format & GenericVertex::FORMAT_TEXCOORDS) texcoords = v.texcoords;
	}
};
#pragma pack(pop)

void bind_vertex(lua_State *L);

}
