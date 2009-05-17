#pragma once

#include <d3d9.h>
#include <boost/cstdint.hpp>
#include <util/boost_com_intrusive_ptr.h>
#include <graphic/vertex.h>
#include <graphic/mesh.h>

namespace graphic {

class static_model;

template<class T = vertex_pnt>
class basic_static_mesh: public mesh {
public:
	typedef T vertex_t;
	typedef boost::uint32_t index_t;

	basic_static_mesh(static_model *model_ptr);
	~basic_static_mesh();

	virtual static_model *get_model();
	virtual void draw(joint *joint_ptr);

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

private:
    size_t vertices_count_;
	boost::intrusive_ptr<IDirect3DVertexBuffer9> vertex_buffer_;
    vertex_t *vertex_buffer_locked_;

    size_t indices_count_;
	boost::intrusive_ptr<IDirect3DIndexBuffer9> index_buffer_;
    index_t *index_buffer_locked_;
};

typedef basic_static_mesh<> static_mesh;
typedef basic_static_mesh<vertex_pnjwt> pnjwt_static_mesh;

};
