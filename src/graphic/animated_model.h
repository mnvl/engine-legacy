#pragma once

#include <graphic/model.h>

namespace graphic
{

class Mesh;
class Renderer;

class AnimatedModel: public Model {
public:
	AnimatedModel(boost::shared_ptr<Renderer> const &renderer_ptr = boost::shared_ptr<Renderer>());
	~AnimatedModel();

	Mesh *create_mesh();

    static void bind(lua_State *L);
};


}
