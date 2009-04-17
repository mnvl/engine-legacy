#pragma once

#include <map>
#include <vector>
#include <engine/graphic/mesh.h>

namespace engine {
namespace graphic {

class MemoryMesh {
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

	MemoryMesh();
	virtual ~MemoryMesh();

	std::vector<vertex> vertices_;
	std::vector<index> indices_;
	std::vector<joint> joints_;
};

}
}