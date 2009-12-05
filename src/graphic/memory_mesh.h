#pragma once

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
    typedef vertex_pnt vertex_t;

	MemoryMesh(MemoryModel *m);
	virtual ~MemoryMesh();

	virtual MeshPtr shallow_copy() const;

	std::vector<vertex_t> const &get_vertex_buffer() const
	{
		return vertices_;
	}

	std::vector<index_t> const &get_index_buffer() const
	{
		return indices_;
	}

	virtual void set_vertices_count(size_t count);
	virtual size_t get_vertices_count();
	virtual void set_indices_count(size_t count);
	virtual void lock();
	virtual void set_positions(math::vec<3> const *buffer);
	virtual void set_normals(math::vec<3> const *buffer);
	virtual void set_joint_indices(math::vec<8, unsigned> const *buffer);
	virtual void set_joint_weights(math::vec<8> const *buffer);
	virtual void set_texcoords(math::vec<2> const *buffer);
	virtual void set_face_indices(unsigned *buffer);
	virtual void unlock();

	void transform_vertices(Joint const *joint_ptr, std::vector<vertex_t> &result) const;

	void draw_static(Joint *joint_ptr) const;
	void draw_animated(Joint *joint_ptr) const;

private:
	#pragma  pack(push, 1)
	struct vertex_weights
	{
		math::vec<8, unsigned> joints;
		math::vec<8> factors;
	};
	#pragma pack(pop)

	std::vector<vertex_t> vertices_;
	std::vector<index_t> indices_;
	std::vector<vertex_weights> weights_;
};

}
