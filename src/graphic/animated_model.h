#pragma once

#include <graphic/model.h>

namespace graphic
{

class mesh;
class renderer;

class animated_model: public model {
public:
	animated_model(renderer *renderer_ptr);
	~animated_model();

	mesh *create_mesh();
};


}
