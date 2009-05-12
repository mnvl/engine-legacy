#pragma once

#include <list>
#include <boost/shared_ptr.hpp>
#include <graphic/joint_influence.h>
#include <graphic/material.h>

namespace graphic {

class model;

class mesh {
public:
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

	joint_influence *get_joint_influence_by_name(std::string const &name);

	virtual model *get_model() const;
	virtual void draw();


protected:
	model *model_ptr_;
};

}

