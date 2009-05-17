#pragma once

#include <d3d9.h>
#include <boost/cstdint.hpp>
#include <math/vec.h>

namespace graphic
{

#pragma pack(push, 1)
struct vertex_pnt
{
	math::vec<3> position;
	math::vec<3> normal;
	math::vec<2> texcoords;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct vertex_pnjwt
{
	math::vec<3> position;
	math::vec<3> normal;
	math::vec<4> joints;
	math::vec<4, boost::uint8_t> weights;
	math::vec<2> texcoords;
};
#pragma pack(pop)

template <class T>
struct vertex_traits;

template<>
struct vertex_traits<vertex_pnt>
{
	static D3DVERTEXELEMENT9 *get_descr();
};

template<>
struct vertex_traits<vertex_pnjwt>
{
	static D3DVERTEXELEMENT9 *get_descr();
};

}
