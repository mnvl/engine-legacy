#pragma once

#include <d3d9.h>
#include <util/boost_com_intrusive_ptr.h>
#include "vertex.h"
#include "vertex_array.h"
#include "index.h"
#include "index_array.h"
#include "mesh.h"

namespace graphic {

class StaticModel;

template<class V = vertex_pnt>
class BasicStaticMesh: public Mesh {
public:
	typedef V vertex_t;

	BasicStaticMesh(Model *model_ptr);
	~BasicStaticMesh();

	virtual MeshPtr shallow_copy() const;

	StaticModel *get_model();
	virtual void draw(Joint *joint_ptr);

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

protected:
	typedef VertexArray<V> vertex_buffer_t;
	typedef IndexArray16 index_buffer_t;

    size_t vertices_count_;
    size_t indices_count_;

	boost::shared_ptr<vertex_buffer_t> vertex_buffer_;
	boost::shared_ptr<index_buffer_t> index_buffer_;

    void draw_triangles_list();
};

typedef BasicStaticMesh<> StaticMesh;
typedef BasicStaticMesh<vertex_pnjwt> PnjwtStaticMesh;

};
