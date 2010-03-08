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

#include <boost/shared_ptr.hpp>
#include "vertex.h"
#include "index.h"
#include "mesh.h"
#include "model.h"

namespace graphic
{

class MemoryModel;
class Renderer;

typedef boost::shared_ptr<class MemoryMesh> MemoryMeshPtr;

class MemoryMesh: public Mesh {
public:
    typedef VERTEX_PTBNT vertex_t;

	#pragma pack(push, 1)
	struct JointWeights
	{
		math::vec<8, unsigned> joints;
		math::vec<8> weights;
	};
	#pragma pack(pop)


	MemoryMesh(MemoryModel *m);
	virtual ~MemoryMesh();

	virtual MeshPtr shallow_copy() const;

	std::vector<vertex_t> const &get_vertex_buffer() const { return vertices_; }
	std::vector<index_t> const &get_index_buffer() const { return indices_; }
	std::vector<JointWeights> const &get_vertex_weights() const { return weights_; }

	virtual void set_vertices_count(size_t count);
	virtual size_t get_vertices_count();
	virtual void set_indices_count(size_t count);
	virtual void lock();
	virtual void set_positions(math::vec<3> const *buffer);
	virtual void set_tangents(math::vec<3> const *buffer);
	virtual void set_binormals(math::vec<3> const *buffer);
	virtual void set_normals(math::vec<3> const *buffer);
	virtual void set_joint_indices(math::vec<8, unsigned> const *buffer);
	virtual void set_joint_weights(math::vec<8> const *buffer);
	virtual void set_texcoords(math::vec<2> const *buffer);
	virtual void set_face_indices(unsigned *buffer);
	virtual void unlock();

	void transform_vertices(Joint const *joint_ptr, std::vector<vertex_t> &result) const;

	void draw_static(Joint *joint_ptr) const;
	void draw_animated(Joint *joint_ptr) const;

	static void bind(lua_State *L);

private:
	std::vector<vertex_t> vertices_;
	std::vector<index_t> indices_;
	std::vector<JointWeights> weights_;
};

}
