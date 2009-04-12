#pragma once

#include <xercesc/dom/DOM.hpp>

namespace engine {
namespace graphic {

class collada_importer {
public:
	void load(char const *filename);

private:
	void process_collada(xercesc::DOMNode const *node);
	void process_library_geometries(xercesc::DOMNode const *node);
};

}
}