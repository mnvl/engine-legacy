#pragma once
//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <math/matrix.h>
#include "material.h"
#include "generic_vertex.h"

namespace graphic {

class Joint;
class MeshShader;
class Model;

class Mesh;

typedef boost::shared_ptr<Mesh> MeshPtr;
typedef boost::weak_ptr<Mesh> MeshWeakPtr;

class Mesh {
public:
	struct Triangles
	{
		MaterialPtr material_ptr;
		size_t minimal_vertex_index;
		size_t vertices_count;
		size_t first_index_offset;
		size_t triangles_count;
	};

	typedef std::vector<Triangles> triangles_list_type;
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

	virtual MeshPtr shallow_copy() const = 0;

	void set_model(Model *model);
	Model *get_model() const;

	virtual void set_vertices(GenericVertex *vertices, GenericVertex::format_t format, size_t count) = 0;
	virtual void set_indices(unsigned *indices, size_t count) = 0;

	virtual void draw(Joint *joint_ptr);

	static void bind(lua_State *L);

protected:
	Model *model_;
	boost::shared_ptr<MeshShader> shader_;
};

}
