#pragma once

#include <d3d9.h>
#include <boost/cstdint.hpp>
#include <vector>
#include <math/vec.h>
#include <math/matrix.h>

namespace engine {
namespace graphic {

class IMesh {
public:
	static DWORD const VERTEX_FORMAT = D3DFVF_XYZ | D3DFVF_TEX2;
	static D3DFORMAT const INDEX_FORMAT = D3DFMT_INDEX16;

#pragma pack(push, 1)
	struct vertex {
		math::vec<3> position;
		math::vec<3> normal;
		math::vec<2> texcoords;
	};
#pragma pack(pop)

	typedef boost::uint16_t index;
	typedef math::vec<3, index> face;

	struct influence {
		index vertex_index;
		math::scalar weight;
	};

	struct joint {
		math::matrix<4, 4> inverted_bind_matrix;
		std::vector<influence> infulences;
	};

	virtual ~IMesh() {}

	virtual size_t get_vertices_count() = 0;
	virtual void resize_vertex_buffer(size_t size) = 0;
	virtual vertex *lock_vertex_buffer() = 0;
	virtual void unlock_vertex_buffer(vertex *buffer) = 0;

	virtual size_t get_indices_count() = 0;
	virtual void resize_index_buffer(size_t size) = 0;
	virtual index *lock_index_buffer() = 0;
	virtual void unlock_index_buffer(index *buffer) = 0;

	virtual size_t get_joints_count() = 0;
	virtual void resize_joints_buffer(size_t size) = 0;
	virtual joint *lock_joints_buffer() = 0;
	virtual void unlock_joints_buffer(joint *buffer) = 0;
};

}
}