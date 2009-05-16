#pragma once

#include <graphic/mesh.h>
#include <graphic/memory_mesh.h>
#include <util/boost_com_intrusive_ptr.h>

namespace graphic {

class static_model;

class static_mesh: public mesh {
public:
	static DWORD const VERTEX_FORMAT = memory_mesh::VERTEX_FORMAT;
	static D3DFORMAT const INDEX_FORMAT = memory_mesh::INDEX_FORMAT;
	typedef memory_mesh::vertex_t vertex_t;
	typedef memory_mesh::index_t index_t;

	static_mesh(static_model *model_ptr);
	~static_mesh();

	virtual static_model *get_model();
	virtual void draw();

	virtual void set_vertices_count(size_t count);
	virtual size_t get_vertices_count();
	virtual void set_indices_count(size_t count);
	virtual void lock();
	virtual void set_positions(math::vec<3> const *buffer);
	virtual void set_normals(math::vec<3> const *buffer);
	virtual void set_texcoords(math::vec<2> const *buffer);
	virtual void set_joint_indices(math::vec<4, unsigned> const *buffer);
	virtual void set_joint_weights(math::vec<4> const *buffer);
	virtual void set_face_indices(unsigned *buffer);
	virtual void unlock();

private:
	boost::intrusive_ptr<IDirect3DVertexBuffer9> vertex_buffer_;
	boost::intrusive_ptr<IDirect3DIndexBuffer9> index_buffer_;
};

};
