
#include <stdexcept>
#include <utility>
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
    local_to_world_.scale(scale_.x / ncols_, 1, scale_.z / nrows_);
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

    int col = int(point_local.x);
    int row = int(point_local.z);

    scalar u = point_local.x - scalar(col);
    scalar v = point_local.z - scalar(row);
    scalar uv = u + v;

    if (uv < EPSILON)
    {
        return at(row, col);
    }

	if (uv <= scalar(1))
    {
		scalar y = at(row, col) * (scalar(1) - uv);

        scalar y1 = y + at(row, col + 1) * uv;
        scalar y2 = y + at(row + 1, col) * uv;

        return y1 + (y2 - y1) * u / uv;
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

        return y1 + (y2 - y1) * u / uv;
    }
}

template<class T>
collision_info<3> heightfield<T>::raytrace(line<3> const &l)
{
    math::line<3> l1(l.A * world_to_local_, l.B * world_to_local_);
    if (l1.A >= l1.B) std::swap(l1.A, l1.B);

    vec<3> const &v = l1.A;
    vec<3> dv = l1.B - l1.A;

    int kx1 = std::max(int(l1.A.x), 0), kx2 = std::min(int(l1.B.x), int(ncols_) - 1);
    int kz1 = std::max(int(l1.A.z), 0), kz2 = std::min(int(l1.B.z), int(nrows_) - 1);

    int kx = kx1;
    scalar tx = (kx - v.x) / dv.x;

    int kz = kz1;
    scalar tz = (kz - v.z) / dv.z;

    vec<3> p = v;

    while (kx <= kx2 && kz <= kz2)
    {
        vec<3> p1;

        if (tx < tz)
        {
            p1 = v + tx * dv;

            kx++;
            tx = (kx - v.x) / dv.x;
        }
        else
        {
            p1 = v + tz * dv;

            kz++;
            tz = (kz - v.z) / dv.z;
        }

        scalar y1 = at(kx + 0, kz + 0);
        scalar y2 = at(kx + 1, kz + 0);
        scalar y3 = at(kx + 1, kz + 1);
        scalar y4 = at(kx + 0, kz + 1);

        scalar ymin = std::min(std::min(y1, y2), std::min(y3, y4));
        scalar ymax = std::max(std::max(y1, y2), std::max(y3, y4));

        if (p.y < ymin && p1.y < ymin || p.y > ymax && p1.y > ymax) {
            p = p1;
            continue;
        }

        vec<3> v1(scalar(kx + 0), y1, scalar(kz + 0));
        vec<3> v2(scalar(kx + 1), y2, scalar(kz + 0));
        vec<3> v3(scalar(kx + 1), y3, scalar(kz + 1));
        vec<3> v4(scalar(kx + 0), y4, scalar(kz + 1));

        ray<3> const &r = line<3>(p, p1).ray();

        triangle<3> T1(v1, v2, v4);
        triangle<3> T2(v1, v2, v4);

        scalar t1, t2;
        vec<3> c1, c2;
        if (T1.trace(r, t1, c1, 0, 1) || T2.trace(r, t2, c2, 0, 1))
        {
            collision_info<3> ci;
            ci.happened = true;
            ci.penetrated = false;

            if (t1 < t2)
            {
                ci.position = c1;
                ci.normal = T1.get_normal();
            }
            else
            {
                ci.position = c2;
                ci.normal = T2.get_normal();
            }
        }
    }

    return collision_info<3>();
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
