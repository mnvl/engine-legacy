
#include <stdexcept>
#include <boost/cstdint.hpp>
#include <platform/file_handle.h>
#include <math/heightfield.h>

namespace math
{

template<class T>
heightfield<T>::heightfield(matrix<4,4> const &tf, size_t nrows, size_t ncols, vec<3> const &scale):
    local_to_world_(tf),
    nrows_(nrows),
    ncols_(ncols),
    scale_(scale)
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
	point_local.x *= nrows_ / scale_.x;
	point_local.z *= nrows_ / scale_.z;

    int col = int(point_local.x);
    int row = int(point_local.z);

    if (row < 0 || row >= (int) nrows_ || col < 0 || col >= (int) ncols_)
        return 0;

    scalar u = point_local.x - scalar(col);
    scalar v = point_local.z - scalar(row);
    scalar uv = u + v;

    if (uv < EPSILON)
    {
        return scale_.y * at(row, col);
    }

	if (uv <= scalar(1))
    {
		scalar y = at(row, col) * (scalar(1) - uv);

        scalar y1 = y + at(row, col + 1) * uv;
        scalar y2 = y + at(row + 1, col) * uv;

        return scale_.y * (y1 + (y2 - y1) * u / uv);
    }
    else
    {
		u = scalar(1) - u;
		v = scalar(1) - v;
		uv = u + v;

		if (uv < EPSILON)
		{
			return at(row + 1, col + 1);
		}

		scalar y = at(row, col) * (scalar(1) - uv);

		scalar y1 = y + at(row, col + 1) * uv;
        scalar y2 = y + at(row + 1, col) * uv;

        return scale_.y * (y1 + (y2 - y1) * u / uv);
    }
}

template<class T>
void heightfield<T>::load_from_buffer(value_type const *heights)
{
    heights_.assign(heights, heights + nheights());
}

template<class T>
void heightfield<T>::load_from_raw_file(std::string const &name)
{
    std::vector<char> buffer;
    platform::file_handle(name.c_str(), O_RDONLY).read_all(buffer);

    if (buffer.size() != nheights() * sizeof(value_type))
        throw std::logic_error("heigtfield " + name + " contains invalid amount of data");

    load_from_buffer((value_type const *) &buffer[0]);
}

template class heightfield<boost::uint8_t>;
template class heightfield<boost::uint16_t>;

}
