#pragma once

#include <d3d9.h>
#include <map>
#include <list>
#include <boost/cstdint.hpp>
#include <math/vec.h>
#include <math/matrix.h>
#include <graphic/model.h>

namespace graphic {

class memory_model;
class renderer;

class memory_mesh: public mesh {
public:
	static DWORD const VERTEX_FORMAT = D3DFVF_XYZ | D3DFVF_TEX2;
	static D3DFORMAT const INDEX_FORMAT = D3DFMT_INDEX16;

#pragma pack(push, 1)
	struct vertex_t {
		math::vec<3> position;
		math::vec<3> normal;
		math::vec<2> texcoords;
	};
#pragma pack(pop)

	typedef boost::uint16_t index_t;
	typedef math::vec<3, index_t> face;
	typedef std::list<joint_influence> influences_type;

	memory_mesh(memory_model *m);
	virtual ~memory_mesh();

	memory_model *model_ptr;
	math::matrix<4,4> bind_shape_matrix;
	std::vector<vertex_t> vertices;
	std::vector<index_t> indices;
	influences_type influences;

	void transform_vertices(joint const *joint_ptr, std::vector<vertex_t> &result) const;

	joint_influence *get_joint_influence_by_name(std::string const &name, bool can_create = false);
	void draw_wireframe(renderer *renderer_ptr, joint *joint_ptr, math::scalar time = 0) const;
};

}
