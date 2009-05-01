#pragma once

#include <d3d9.h>
#include <map>
#include <list>
#include <boost/cstdint.hpp>
#include <math/vec.h>
#include <math/matrix.h>
#include <graphic/model.h>

namespace engine {
namespace graphic {

class memory_model;
class renderer;

class memory_mesh {
public:
	typedef model::joint joint;
	typedef model::joint_influence joint_influence;

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
	typedef std::list<joint_influence> influences_type;

	memory_mesh(memory_model *model_ptr);
	virtual ~memory_mesh();

	memory_model *model_ptr_;
	std::string id, name;
	math::matrix<4,4> transform;
	std::vector<vertex> vertices;
	std::vector<index> indices;
	influences_type influences;

	void transform_vertices(std::vector<vertex> &result) const;

	joint_influence *get_joint_influence_by_name(std::string const &name, bool can_create = false);
	void draw_wireframe(renderer *renderer_ptr, math::scalar time = 0) const;
};

}
}