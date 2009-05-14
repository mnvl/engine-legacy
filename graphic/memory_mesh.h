#pragma once

#include <graphic/mesh.h>
#include <graphic/model.h>

namespace graphic {

class memory_model;
class renderer;

class memory_mesh: public mesh {
public:
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

	void set_vertex_buffer(vertex_t *vertices, size_t count);
	void set_index_buffer(index_t *indices, size_t count);

	void transform_vertices(joint const *joint_ptr, std::vector<vertex_t> &result) const;

	void draw_static(joint *joint_ptr) const;
	void draw_animated(joint *joint_ptr) const;

private:
	std::vector<vertex_t> vertices_;
	std::vector<index_t> indices_;

};

}
