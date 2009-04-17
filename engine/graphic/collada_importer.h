#pragma once

#include <map>
#include <vector>
#include <xercesc/dom/DOM.hpp>
#include <math/vec.h>
#include <engine/graphic/mesh.h>
#include <engine/graphic/memory_mesh.h>

namespace engine {
namespace graphic {

class collada_importer {
public:
	collada_importer(std::string const &filename);
	void load();

private:
	std::string filename_;

	struct technique_info {
		size_t count, stride;
	};

	struct source_info {
		std::vector<math::scalar> data;
		technique_info technique;

		template<size_t N>
		void fill_vector(math::vec<N> &v, size_t index) const {
			size_t offset = technique.stride * index;
			float const *get = &data[offset];
			for (size_t i = 0; i < N; i++) v.i[i] = *get++;
		}
	};

	struct input_info {
		std::string source_name;
		size_t offset;
	};

	struct mesh_info {
		struct vertex_comparator {
			bool operator ()(MemoryMesh::vertex const &lhs, MemoryMesh::vertex const &rhs) {
				if (lhs.position < rhs.position) return true;
				if (lhs.position > rhs.position) return false;

				if (lhs.normal < rhs.normal) return true;
				if (lhs.normal > rhs.normal) return false;

				return lhs.texcoords < rhs.texcoords;
			}
		};

		typedef std::map<MemoryMesh::vertex, MemoryMesh::index, vertex_comparator> verices_inverted_index_type;
		verices_inverted_index_type vertices_inverted_index;
		std::map<MemoryMesh::index, MemoryMesh::index> position_index_transform;

		MemoryMesh::index getIndexForVertex(MemoryMesh::vertex const &v, MemoryMesh::index original_index) {
			std::pair<verices_inverted_index_type::const_iterator, bool> result =
				vertices_inverted_index.insert(std::make_pair(v, vertices_inverted_index.size()));

			if (result.second) {
				mesh.vertices_.push_back(v);
				position_index_transform.insert(std::make_pair(original_index, result.first->second));
			}

			return result.first->second;
		}

		MemoryMesh mesh;
	};

	std::map<std::string, mesh_info> builded_meshes_;
	MemoryMesh *mesh_ptr_;
	mesh_info *mesh_info_;

	// name to source_info
	std::map<std::string, source_info> sources_;

	// semantic to source info
	std::map<std::string, input_info> inputs_;

	void clear();
	source_info const &indirect_source(xercesc::DOMNode const *input_node);
	void process_collada(xercesc::DOMNode const *node);
	void process_library_geometries(xercesc::DOMNode const *node);
	void process_geometry(xercesc::DOMNode const *node);
	void process_mesh(xercesc::DOMNode const *node);
	void process_vertices(xercesc::DOMNode const *node);
	void process_inputs(xercesc::DOMNode const *node);
	void process_source(xercesc::DOMNode const *node);
	void process_technique(xercesc::DOMNode const *node, technique_info *si);
	void process_triangles(xercesc::DOMNode const *node);
	void build_mesh(std::vector<unsigned> const &indices);
	void process_library_controllers(xercesc::DOMNode const *node);
	void process_controller(xercesc::DOMNode const *node);
	void process_skin(xercesc::DOMNode const *node);
	void process_joints(xercesc::DOMNode const *node);
	void process_vertex_weights(xercesc::DOMNode const *node);
	void process_library_animations(xercesc::DOMNode const *node);
	void process_animation(xercesc::DOMNode const *node);
	void process_sampler(xercesc::DOMNode const *node);
};

}
}