#pragma once

#include <boost/cstdint.hpp>
#include <util/boost_com_intrusive_ptr.h>
#include <graphic/vertex.h>
#include <graphic/mesh.h>

namespace graphic {

class static_model;

class static_mesh: public mesh {
public:
	typedef vertex_pnt vertex_t;
	typedef boost::uint16_t index_t;

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
