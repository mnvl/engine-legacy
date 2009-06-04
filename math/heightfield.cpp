
#include <math/heightfield.h>

namespace math
{

template<class T>
heightfield<T>::heightfield(matrix<4,4> const &tf, size_t nrows, size_t ncols,
    vec<3> const &scale, value_type const *heights):
local_to_world_(tf),
nrows_(nrows),
ncols_(ncols),
scale_(scale),
heights_(heights_, nrows * ncols)
{
    local_to_world_.inverse(world_to_local_);
}

template<class T>
heightfield<T>::~heightfield()
{
}

template<class T>
scalar heightfield<T>::y_at(vec<3> const &point)
{
    vec<3> point_local = point * world_to_local_;

    int row = int(point_local.x * nrows_);
    int col = int(point_local.z * ncols_);

    if (row < 0 || row_ >= nrows_ || col_ < 0 || col_ >= ncols_)
        return 0;

    scalar u = point.x - scalar(row);
    scalar v = point.z - scalar(col);

    scalar inv_u = scalar(1) - u;
    scalar inv_v = scalar(1) - v;

    scalar y1, y2;

    if (square(x) + square(z) > scalar(1))
    {
        y1 = at(row, col) * u + at(row + 1, col) * inv_u;
        y2 = at(row, col) * v + at(row, col + 1) * inv_v;
    }
    else
    {
        y1 = at(row + 1, col + 1) * inv_u + at(row + 1, col) * u;
        y2 = at(row + 1, col + 1) * inv_v + at(row, col + 1) * v;
    }
}

}
