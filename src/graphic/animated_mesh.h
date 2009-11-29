#pragma once

#include <graphic/static_mesh.h>

namespace graphic
{

class AnimatedModel;

class AnimatedMesh: public PnjwtStaticMesh {
public:
    AnimatedMesh(Model *model_ptr);

    AnimatedModel *get_model();
    virtual void draw(Joint *joint_ptr);

private:
	boost::shared_ptr<Shader> shader_;
};

}
