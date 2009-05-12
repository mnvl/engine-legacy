#pragma once

#include <graphic/model.h>
#include <graphic/memory_model.h>
#include <graphic/static_mesh.h>
#include <graphic/renderer.h>

namespace graphic {

class static_model: public model {
public:
	static_model(renderer *renderer_ptr);
	~static_model();

	void build_from_memory_model(memory_model *model_ptr);
	void draw();
};

}

