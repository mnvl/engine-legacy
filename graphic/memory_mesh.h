#pragma once

#include <boost/cstdint.hpp>
#include <graphic/mesh.h>
#include <graphic/model.h>

namespace graphic {

class memory_model;
class renderer;

class memory_mesh: public mesh {
public:
	static DWORD const VERTEX_FORMAT = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2;
	static D3DFORMAT const INDEX_FORMAT = D3DFMT_INDEX16;

	#pragma pack(push, 1)
	struct vertex_t {
		math::vec<3> position;
		math::vec<3> normal;
		math::vec<2> texcoords;
	};
	#pragma pack(pop)

	typedef boost::uint16_t index_t;

	memory_mesh(memory_model *m);
	virtual ~memory_mesh();

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
	virtual void set_texcoords(math::vec<2> const *buffer);
	virtual void set_joint_indices(math::vec<4, unsigned> const *buffer);
	virtual void set_joint_weights(math::vec<4> const *buffer);
	virtual void set_face_indices(unsigned *buffer);
	virtual void unlock();

	void transform_vertices(joint const *joint_ptr, std::vector<vertex_t> &result) const;

	void draw_static(joint *joint_ptr) const;
	void draw_animated(joint *joint_ptr) const;

private:
	#pragma  pack(push, 1)
	struct vertex_weights 
	{
		math::vec<4, unsigned> joints;
		math::vec<4> factors;
	};
	#pragma pack(pop)
	
	std::vector<vertex_t> vertices_;
	std::vector<index_t> indices_;
	std::vector<vertex_weights> weights_;
};

}
