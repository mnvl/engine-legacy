#pragma once

#include <set>
#include <map>
#include <vector>
#include <stdexcept>
#include <xercesc/dom/DOM.hpp>
#include <math/vec.h>
#include <graphic/model.h>

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

	collada_loader(std::string const &filename, model *model_ptr, renderer *renderer_ptr = 0);
	~collada_loader();

	void load();

private:
	std::string filename_;
	model *model_ptr_;
	renderer *renderer_ptr_;
};

}
