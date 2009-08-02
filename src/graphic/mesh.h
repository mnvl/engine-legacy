#pragma once

#include <vector>
#include <boost/shared_ptr.hpp>
#include <math/matrix.h>
#include <graphic/material.h>

namespace graphic {

class Joint;
class Model;

class Mesh {
public:
	struct triangles
	{
		Material *material_ptr;
		size_t first_index_offset;
		size_t triangles_count;
		size_t minimal_vertex_index;
		size_t vertices_count;
	};

	typedef std::vector<triangles> triangles_list_type;
	typedef std::vector<math::matrix<4,4> > inverted_bind_matrices_type;
    typedef std::vector<size_t> joint_index_transform_type;

	std::string id, name;
	triangles_list_type triangles_list;
	math::matrix<4,4> bind_shape_matrix;
	inverted_bind_matrices_type inverted_bind_matrices;

    // for all joints influenting on vertices of this mesh this vector
    // contains index of joint in model's joints array
    joint_index_transform_type joint_index_transform;

	Mesh(Model *model_ptr = 0);
	virtual ~Mesh();

	Model *get_model() const;

	virtual void set_vertices_count(size_t count) = 0;
	virtual size_t get_vertices_count() = 0;
	virtual void set_indices_count(size_t count) = 0;
	virtual void lock() = 0;
	virtual void set_positions(math::vec<3> const *buffer) = 0;
	virtual void set_normals(math::vec<3> const *buffer) = 0;

    // every element of buffer contains indices of elements from joint_index_transform,
    // which points further to model's joint
	virtual void set_joint_indices(math::vec<8, unsigned> const *buffer) = 0;
	virtual void set_joint_weights(math::vec<8> const *buffer) = 0;

	virtual void set_texcoords(math::vec<2> const *buffer) = 0;
	virtual void set_face_indices(unsigned *buffer) = 0;
	virtual void unlock() = 0;

	virtual void draw(Joint *joint_ptr);

protected:
	Model *model_ptr_;
};

}

