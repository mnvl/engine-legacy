#pragma once

#include <xercesc/dom/DOM.hpp>

namespace engine {
namespace graphic {

class collada_importer {
public:
	collada_importer(std::string const &filename);
	void load();

private:
	void process_collada(xercesc::DOMNode const *node);
	void process_library_geometries(xercesc::DOMNode const *node);
	void process_geometry(xercesc::DOMNode const *node);
	void process_mesh(xercesc::DOMNode const *node);
};

}
}