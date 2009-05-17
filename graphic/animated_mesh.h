#pragma once

#include <graphic/static_mesh.h>

namespace graphic
{

class animated_mesh: public pnjwt_static_mesh {
public:
    virtual void draw(joint *joint_ptr);
};

}
