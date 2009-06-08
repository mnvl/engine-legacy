#pragma once

#include <math/vec.h>
#include <math/line.h>
#include <math/matrix.h>
#include <math/collision.h>
#include <vector>

namespace math
{

template<class T>
class heightfield {
public:
    typedef T value_type;

    heightfield(matrix<4,4> const &tf, size_t nrows, size_t ncols, vec<3> const &scale);
    ~heightfield();

    scalar at(int row, int col)
    {
        return heights_.at(row * ncols_ + col) * scale_.y;
    }

    // world to local transform aplyed on point, so we supply all three components of point
    // result is undefined if point is outside heightfield bounds
    scalar y_at(vec<3> const &point);

    collision_info<3> trace(ray<3> const &r, scalar max_distance = 1000.0f);

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
