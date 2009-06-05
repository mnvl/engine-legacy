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

    heightfield(matrix<4,4> const &tf, size_t nrows, size_t ncols, vec<3> const &scale);
    ~heightfield();

    math::scalar at(int row, int col)
    {
        return heights_.at(row * ncols_ + col);
    }

    // world to local transform aplyed on point, so we supply all three components of point
    // returns 0 if point is outside heightfield
    scalar y_at(vec<3> const &point);

    void load_from_buffer(value_type const *heights);
    void load_from_raw_file(std::string const &name);

private:
    matrix<4,4> local_to_world_, world_to_local_;
    size_t nrows_, ncols_;
    vec<3> scale_;
    std::vector<value_type> heights_;

    size_t nheights()
    {
        return nrows_ * ncols_;
    }
};

}
