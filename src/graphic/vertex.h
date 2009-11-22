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
	math::vec<8, boost::uint8_t> joints;
	math::vec<8> weights;
	math::vec<2> texcoords;
};
#pragma pack(pop)

template <class T>
struct vertex_traits;

template<>
struct vertex_traits<vertex_pnt>
{
	static D3DVERTEXELEMENT9 *get_descr();
	static size_t get_size() { return sizeof(vertex_pnt); }
};

template<>
struct vertex_traits<vertex_pnjwt>
{
	static D3DVERTEXELEMENT9 *get_descr();
	static size_t get_size() { return sizeof(vertex_pnjwt); }
};

void bind_vertex(lua_State *L);

}
