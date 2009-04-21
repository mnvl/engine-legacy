#pragma once

#include <set>
#include <map>
#include <vector>
#include <stdexcept>
#include <xercesc/dom/DOM.hpp>
#include <math/vec.h>
#include <engine/graphic/memory_model.h>

namespace engine {
namespace graphic {

class collada_loader {
public:
	class load_error: public std::runtime_error {
	public:
		load_error(std::string const &message):
		runtime_error("collada loading error: " + message)
		{
		}
	};

	collada_loader(std::string const &filename);
	~collada_loader();
	void load();

private:
	std::string filename_;
};

}
}