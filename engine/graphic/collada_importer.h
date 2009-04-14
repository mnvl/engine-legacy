#pragma once

#include <map>
#include <vector>
#include <xercesc/dom/DOM.hpp>
#include <math/vec.h>
#include <engine/graphic/mesh.h>

namespace engine {
namespace graphic {

class collada_importer {
public:
	collada_importer(std::string const &filename, mesh *mesh_ptr);
	void load();

private:
	std::string filename_;
	mesh *mesh_ptr_;

	std::map<std::string, xercesc::DOMNode const *> sources_;

	struct technique_info {
		size_t count, stride;
	};

	struct source_info {
		std::vector<math::scalar> data;
		technique_info technique;
	};

	source_info positions_;
	source_info normals_;
	source_info texcoords_;

	void process_collada(xercesc::DOMNode const *node);
	void process_library_geometries(xercesc::DOMNode const *node);
	void process_geometry(xercesc::DOMNode const *node);
	void process_mesh(xercesc::DOMNode const *node);
	void process_vertices(xercesc::DOMNode const *node);
	void process_source(xercesc::DOMNode const *node, source_info *si);
	void process_technique(xercesc::DOMNode const *node, technique_info *si);
};

}
}