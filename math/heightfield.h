#pragma once

#include <math/vec.h>
#include <math/matrix.h>
#include <vector>

namespace math
{

template<class T>
class heightfield {
public:
    typedef T value_type;

    heightfield(matrix<4,4> const &tf, size_t nrows, size_t ncols, vec<3> const &scale, value_type const *heights);
    ~heightfield();

    math::scalar at(int row, int col)
    {
        return heights_.at(row * nrows_ + col);
    }

    // world to local transform aplyed on point, so we supply all three components of point
    // returns 0 if point is outside heightfield
    scalar y_at(vec<3> const &point);

private:
    matrix<4,4> local_to_world_, world_to_local_;
    size_t nrows_, ncols_;
    vec<3> scale_;
    std::vector<value_type> heights_;
};

}
