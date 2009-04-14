#pragma once

#include <d3d9.h>
#include <engine/math/vec.h>

class mesh {
public:
	static DWORD const VERTEX_FORMAT = D3DFVF_XYZ | D3DFVF_TEX2;

#pragma pack(push, 1)
	struct vertex {
		math::vec<3> position;
		math::vec<3> normal;
		math::vec<2> texcoords;
	};
#pragma pack(pop)

	virtual size_t get_vertices_count() = 0;
	virtual void resize_vertex_buffer(size_t size) = 0;
	virtual vertex *get_vertex_buffer() = 0;
};