#pragma once

#include <map>
#include <vector>
#include <xercesc/dom/DOM.hpp>
#include <math/vec.h>
#include <engine/graphic/mesh.h>
#include <engine/graphic/buildable_mesh.h>

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

	// id to buildable mesh
	std::map<std::string, buildable_mesh> builded_meshes_;
	buildable_mesh *mesh_ptr_;

	// name to source_info
	std::map<std::string, source_info> sources_;

	// semantic to source info
	std::map<std::string, input_info> inputs_;

	void clear();
	void process_collada(xercesc::DOMNode const *node);
	void process_library_geometries(xercesc::DOMNode const *node);
	void process_geometry(xercesc::DOMNode const *node);
	void process_mesh(xercesc::DOMNode const *node);
	void process_vertices(xercesc::DOMNode const *node);
	void process_source(xercesc::DOMNode const *node, source_info *si);
	void process_technique(xercesc::DOMNode const *node, technique_info *si);
	void process_triangles(xercesc::DOMNode const *node);
	void process_library_controllers(xercesc::DOMNode const *node);
	void process_controller(xercesc::DOMNode const *node);
	void process_skin(xercesc::DOMNode const *node);
	void build_mesh(std::vector<unsigned> const &indices);
};

}
}