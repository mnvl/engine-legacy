
#include <engine/graphic/memory_mesh.h>

namespace engine {
namespace graphic {

memory_mesh::memory_mesh() {
}

memory_mesh::~memory_mesh() {
}

size_t memory_mesh::get_vertices_count() {
	return vertices_.size();
}

void memory_mesh::resize_vertex_buffer(size_t size) {
	vertices_.resize(size);
}

mesh::vertex *memory_mesh::lock_vertex_buffer() {
	return &vertices_[0];
}

void memory_mesh::unlock_vertex_buffer(vertex *buffer) {
}

size_t memory_mesh::get_indices_count() {
	return indices_.size();
}

void memory_mesh::resize_index_buffer(size_t size) {
	indices_.resize(size);
}

mesh::index *memory_mesh::lock_index_buffer() {
	return &indices_[0];
}

void memory_mesh::unlock_index_buffer(index *buffer) {
}

size_t memory_mesh::get_joints_count() {
	return joints_.size();
}

void memory_mesh::resize_joints_buffer(size_t size) {
	joints_.resize(size);
}

mesh::joint *memory_mesh::lock_joints_buffer() {
	return &joints_[0];
}

void memory_mesh::unlock_joints_buffer(joint *joints) {
}

mesh::index memory_mesh::add_vertex(vertex const &v) {
	std::pair<verices_inverted_index_type::const_iterator, bool> result =
		vertices_inverted_index_.insert(std::make_pair(v, vertices_.size()));

	if (result.second) {
		vertices_.push_back(v);
		return vertices_.size() - 1;
	}

	return result.first->second;
}

void memory_mesh::add_index(index i) {
	indices_.push_back(i);
}

}
}