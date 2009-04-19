#pragma once

#include <map>
#include <vector>
#include <stdexcept>
#include <xercesc/dom/DOM.hpp>
#include <math/vec.h>
#include <engine/graphic/memory_model.h>

namespace engine {
namespace graphic {

class collada_importer {
public:
	class import_error: public std::runtime_error {
	public:
		import_error(std::string const &message):
		runtime_error("import error: " + message)
		{
		}
	};

	collada_importer(std::string const &filename);
	void load();
	memory_model const &get_model() const { return model_; }

private:
	std::string filename_;

	struct technique_info {
		typedef std::map<std::string, std::string> params_type;	// name to type

		params_type params;
		size_t count, stride;
	};

	struct source_info {
		std::vector<math::scalar> float_array;
		std::vector<std::string> name_array;
		technique_info technique;

		template<size_t N>
		void fill_vector(math::vec<N> &v, size_t index) const {
			size_t offset = technique.stride * index;
			float const *get = &float_array[offset];
			for (size_t i = 0; i < N; i++) v.i[i] = *get++;
		}
	};

	struct input_info {
		std::string source_name;
		size_t offset;
	};

	struct mesh_in_progress {
		struct vertex_comparator {
			bool operator ()(memory_mesh::vertex const &lhs, memory_mesh::vertex const &rhs) {
				if (lhs.position <= rhs.position) return true;
				if (lhs.position >= rhs.position) return false;

				if (lhs.normal <= rhs.normal) return true;
				if (lhs.normal >= rhs.normal) return false;

				return lhs.texcoords <= rhs.texcoords;
			}
		};

		memory_mesh *mesh_ptr;

		typedef std::map<memory_mesh::vertex, memory_mesh::index, vertex_comparator> verices_inverted_index_type;
		typedef std::multimap<memory_mesh::index, memory_mesh::index> position_index_transform_type;

		verices_inverted_index_type vertices_inverted_index;
		position_index_transform_type position_index_transform;

		mesh_in_progress(memory_mesh *mesh = 0):
		mesh_ptr(mesh)
		{
		}

		memory_mesh::index getIndexForVertex(memory_mesh::vertex const &v, memory_mesh::index original_index) {
			memory_mesh::index index_value = vertices_inverted_index.size();

			std::pair<verices_inverted_index_type::const_iterator, bool> result =
				vertices_inverted_index.insert(std::make_pair(v, index_value));

			if (result.second) {
				mesh_ptr->vertices.push_back(v);
				position_index_transform.insert(std::make_pair(original_index, index_value));
				return index_value;
			}

			return result.first->second;
		}
	};

	// name to mesh in progress
	std::map<std::string, mesh_in_progress> meshes_in_progress_;

	// instance controller name to mesh name
	std::map<std::string, std::string> mesh_by_instance_controller_;

	// animation by name
	std::map<std::string, sampler> sampler_by_name_;

	mesh_in_progress *mesh_in_progress_;
	memory_model model_;

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
	void process_accessor(xercesc::DOMNode const *node, technique_info *ti);
	void process_triangles(xercesc::DOMNode const *node);
	void process_library_controllers(xercesc::DOMNode const *node);
	void process_controller(xercesc::DOMNode const *node);
	void process_skin(xercesc::DOMNode const *node);
	void process_joints(xercesc::DOMNode const *node);
	void process_vertex_weights(xercesc::DOMNode const *node);
	void process_library_visual_scenes(xercesc::DOMNode const *node);
	void process_visual_scene(xercesc::DOMNode const *node);
	void process_node(xercesc::DOMNode const *node, joint *parent = 0);
	void process_library_animations(xercesc::DOMNode const *node);
	void process_animation(xercesc::DOMNode const *node);
	void process_sampler(xercesc::DOMNode const *node);
	void build_mesh(std::vector<unsigned> const &indices);
};

}
}