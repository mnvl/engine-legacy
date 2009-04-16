
#include <engine/graphic/buildable_mesh.h>

namespace engine {
namespace graphic {

buildable_mesh::buildable_mesh() {
}

buildable_mesh::~buildable_mesh() {
}

size_t buildable_mesh::get_vertices_count() {
	return vertices_.size();
}

void buildable_mesh::resize_vertex_buffer(size_t size) {
	vertices_.resize(size);
}

mesh::vertex *buildable_mesh::lock_vertex_buffer() {
	return &vertices_[0];
}

void buildable_mesh::unlock_vertex_buffer(vertex *buffer) {
}

size_t buildable_mesh::get_indices_count() {
	return indices_.size();
}

void buildable_mesh::resize_index_buffer(size_t size) {
	indices_.resize(size);
}

mesh::index *buildable_mesh::lock_index_buffer() {
	return &indices_[0];
}

void buildable_mesh::unlock_index_buffer(index *buffer) {
}

void buildable_mesh::add_face(vertex const *face_vertices, size_t count) {
	for (size_t i = 0; i < count; i++) {
		std::pair<verices_inverted_index_type::const_iterator, bool> result =
			vertices_inverted_index_.insert(std::make_pair(face_vertices[i], vertices_.size()));

		if (result.second) {
			indices_.push_back(vertices_.size());
			vertices_.push_back(face_vertices[i]);
		}
		else
		{
			indices_.push_back(result.first->second);
		}
	}
}

}
}