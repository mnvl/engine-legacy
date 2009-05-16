#pragma once

#include <list>
#include <boost/shared_ptr.hpp>
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

	typedef std::vector<triangles> triangles_list_type;

	std::string id, name;
	triangles_list_type triangles_list;
	math::matrix<4,4> bind_shape_matrix;

	mesh(model *model_ptr = 0);
	virtual ~mesh();

	virtual model *get_model() const;

	virtual void set_vertices_count(size_t count) = 0;
	virtual void set_indices_count(size_t count) = 0;
	virtual void lock() = 0;
	virtual void set_positions(math::vec<3> const *buffer) = 0;
	virtual void set_normals(math::vec<3> const *buffer) = 0;
	virtual void set_texcoords(math::vec<2> const *buffer) = 0;
	virtual void set_joint_indices(math::vec<4, unsigned> const *buffer) = 0;
	virtual void set_joint_weights(math::vec<4> const *buffer) = 0;
	virtual void set_face_indices(unsigned *buffer) = 0;
	virtual void unlock() = 0;

	virtual void draw();

protected:
	model *model_ptr_;
};

}

