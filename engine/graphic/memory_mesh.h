#pragma once

#include <d3d9.h>
#include <map>
#include <vector>
#include <boost/cstdint.hpp>
#include <math/vec.h>
#include <math/matrix.h>
#include <engine/graphic/skeletal_animation.h>

namespace engine {
namespace graphic {

class renderer;

class memory_mesh {
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
	typedef std::vector<joint_influence> influences_type;

	memory_mesh();
	virtual ~memory_mesh();

	std::string name;
	math::matrix<4,4> transform;
	std::vector<vertex> vertices;
	std::vector<index> indices;
	influences_type influences;

	void draw_wireframe(renderer *renderer_ptr, math::scalar time = 0) const;
};

}
}