#pragma once

#include <map>
#include <vector>
#include <engine/graphic/mesh.h>

namespace engine {
namespace graphic {

class memory_mesh: public mesh {
public:
	memory_mesh();
	virtual ~memory_mesh();

	virtual size_t get_vertices_count();
	virtual void resize_vertex_buffer(size_t size);
	virtual vertex *lock_vertex_buffer();
	virtual void unlock_vertex_buffer(vertex *buffer);

	virtual size_t get_indices_count();
	virtual void resize_index_buffer(size_t size);
	virtual index *lock_index_buffer();
	virtual void unlock_index_buffer(index *buffer);

	void add_face(vertex const *face_vertices, size_t count);

private:
	std::vector<mesh::vertex> vertices_;
	std::vector<index> indices_;

	// vertex to index in vertex array
	struct vertex_comparator {
		bool operator ()(vertex const &lhs, vertex const &rhs) {
			if (lhs.position < rhs.position) return true;
			if (lhs.position > rhs.position) return false;

			if (lhs.normal < rhs.normal) return true;
			if (lhs.normal > rhs.normal) return false;

			return lhs.texcoords < rhs.texcoords;
		}
	};

	typedef std::map<mesh::vertex, index, vertex_comparator> verices_inverted_index_type;
	verices_inverted_index_type vertices_inverted_index_;
};

}
}