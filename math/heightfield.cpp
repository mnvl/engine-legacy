
#include <boost/cstdint.hpp>
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
heights_(heights, heights + nrows * ncols)
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

    int row = int(point_local.x * nrows_ / scale_.x);
    int col = int(point_local.z * ncols_ / scale_.z);

    if (row < 0 || row >= (int) nrows_ || col < 0 || col >= (int) ncols_)
        return 0;

    scalar u = point.x - scalar(row);
    scalar v = point.z - scalar(col);
    scalar uv = u + v;

    if (uv < EPSILON)
    {
        return at(row, col);
    }
    else if (uv <= scalar(1))
    {
        scalar inv_uv = scalar(1) - uv;

        scalar y1 = at(row, col) * inv_uv + at(row, col + 1) * uv;
        scalar y2 = at(row, col) * inv_uv + at(row + 1, col) * uv;

        return scale_.y * (y1 + (y2 - y1) * v / uv);
    }
    else
    {
        return 0;
    }
}

template class heightfield<boost::uint8_t>;
template class heightfield<boost::uint16_t>;

}
