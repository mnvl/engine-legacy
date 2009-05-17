#pragma once

#include <graphic/static_mesh.h>

namespace graphic
{

class animated_model;

class animated_mesh: public pnjwt_static_mesh {
public:
    animated_mesh(model *model_ptr);

    animated_model *get_model();
    virtual void draw(joint *joint_ptr);
};

}
