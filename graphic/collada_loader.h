#pragma once

#include <set>
#include <map>
#include <vector>
#include <stdexcept>
#include <xercesc/dom/DOM.hpp>
#include <math/vec.h>
#include <graphic/memory_model.h>

namespace graphic {

class renderer;

class collada_loader {
public:
	class load_error: public std::runtime_error {
	public:
		load_error(std::string const &message):
		runtime_error(message)
		{
		}
	};

	collada_loader(std::string const &filename, renderer *renderer_ptr = 0);
	~collada_loader();

	memory_model &get_model() { return model_; }

	void load();

private:
	std::string filename_;

	renderer *renderer_ptr_;
	memory_model model_;
};

}
