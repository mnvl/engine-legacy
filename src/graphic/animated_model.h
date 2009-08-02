#pragma once

#include <graphic/model.h>

namespace graphic
{

class Mesh;
class Renderer;

class AnimatedModel: public Model {
public:
	AnimatedModel(Renderer *renderer_ptr);
	~AnimatedModel();

	Mesh *create_mesh();
};


}
