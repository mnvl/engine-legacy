#pragma once

#include <vector>
#include <math/vec.h>

namespace graphic
{

void build_smooth_normals(std::vector<math::vec<3> > const &positions, std::vector<unsigned> const &faces, std::vector<math::vec<3> > *normals);

}
