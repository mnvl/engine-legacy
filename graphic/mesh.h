#pragma once

#include <list>
#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>
#include <graphic/joint_influence.h>
#include <graphic/material.h>

namespace graphic {

class model;

class mesh {
public:
	static DWORD const VERTEX_FORMAT = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2;
	static D3DFORMAT const INDEX_FORMAT = D3DFMT_INDEX16;

	#pragma pack(push, 1)
	struct vertex_t {
		math::vec<3> position;
		math::vec<3> normal;
		math::vec<2> texcoords;
	};
	#pragma pack(pop)

	typedef boost::uint16_t index_t;

	struct triangles
	{
		material *material_ptr;
		size_t first_index_offset;
		size_t triangles_count;
		size_t minimal_vertex_index;
		size_t vertices_count;
	};

	typedef std::list<joint_influence> influences_type;
	typedef std::vector<triangles> triangles_list_type;

	std::string id, name;
	triangles_list_type triangles_list;
	math::matrix<4,4> bind_shape_matrix;
	influences_type influences;

	mesh(model *model_ptr = 0);
	virtual ~mesh();

	virtual model *get_model() const;
	joint_influence *get_joint_influence_by_name(std::string const &name);

	virtual void set_vertex_buffer(vertex_t *vertices, size_t count) = 0;
	virtual void set_index_buffer(index_t *indices, size_t count) = 0;

	virtual void draw();

protected:
	model *model_ptr_;
};

}

