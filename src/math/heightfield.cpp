
#include <stdexcept>
#include <utility>
#include <boost/cstdint.hpp>
#include <util/file_handle.h>
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
contact_info<3> heightfield<T>::trace(ray<3> const &r, scalar max_distance)
{
    // x0 + dx * t = x(t)
    // t(x) = (x - x0) / dx = x * 1 / dx - x0 / dx

    line<3> l(r.origin * world_to_local_,
		(r.origin + normalize(r.direction) * max_distance) * world_to_local_);
    if (lexicagraphical_greater(l.A, l.B)) std::swap(l.A, l.B);

    vec<3> const &v0 = l.A;
    vec<3> dv = l.B - l.A;

	if (abs(dv.x) < EPSILON && abs(dv.z) < EPSILON) {
		scalar y = y_at(r.origin);

		if (y * r.direction.y > 0)
		{
			contact_info<3> ci;
			ci.happened = true;
            ci.penetrated = false;
			ci.position.set(r.origin.x, y, r.origin.z);

			int col = int(ci.position.x), row = int(ci.position.z);

	        vec<3> v1 = vec<3>(scalar(row + 0), at(row + 0, col + 0), scalar(col + 0));
		    vec<3> v2 = vec<3>(scalar(row + 0), at(row + 0, col + 1), scalar(col + 1));
			vec<3> v3 = vec<3>(scalar(row + 1), at(row + 1, col + 1), scalar(col + 1));
			vec<3> v4 = vec<3>(scalar(row + 1), at(row + 1, col + 0), scalar(col + 0));

			triangle<3> tri1(v1, v2, v4);
			triangle<3> tri2(v3, v4, v2);

			if (tri1.contains_on_plane(ci.position)) ci.normal = tri1.get_normal();
			else ci.normal = tri2.get_normal();

			ci.time = (ci.position - r.origin).length() / r.direction.length();

			return ci;
		}
		else
		{
			contact_info<3> ci;
			ci.happened = false;
			return ci;
		}
	}

    scalar tx = -v0.x / dv.x, tz = -v0.z / dv.z;
	scalar inv_dx = abs(dv.x) > EPSILON ? 1 / dv.x : 0, inv_dz = abs(dv.z) > EPSILON ? 1 / dv.z : 0;

	if (tx < 0) tx += (1 + floor(-tx / inv_dx)) * inv_dx;
    if (tz < 0) tz += (1 + floor(-tz / inv_dz)) * inv_dz;

	vec<3> prev_p, next_p = v0;

    while ((tx <= 1 || inv_dx == 0) && (tz <= 1 || inv_dz == 0))
    {
        prev_p = next_p;

        if ((inv_dx && !inv_dz) || tx < tz)
        {
            next_p = v0 + dv * tx;
            tx += inv_dx;
        }
        else
        {
            next_p = v0 + dv * tz;
            tz += inv_dz;
        }

		if ((prev_p - next_p).length_sq() < math::EPSILON) continue;

		int col = std::min(std::max(0, int(prev_p.x)), int(next_p.x)),
			row = std::min(std::max(0, int(prev_p.z)), int(next_p.z));

		if (col == ncols_ - 1 || row == nrows_ - 1) break;

        scalar y1 = at(row + 0, col + 0);
        scalar y2 = at(row + 0, col + 1);
        scalar y3 = at(row + 1, col + 1);
        scalar y4 = at(row + 1, col + 0);

        scalar ymin = std::min(std::min(y1, y2), std::min(y3, y4));
        scalar ymax = std::max(std::max(y1, y2), std::max(y3, y4));

        if ((prev_p.y < ymin && next_p.y < ymin) || (next_p.y > ymax && next_p.y > ymax))
            continue;

        vec<3> v1 = vec<3>(scalar(row + 0), y1, scalar(col + 0));
        vec<3> v2 = vec<3>(scalar(row + 0), y2, scalar(col + 1));
        vec<3> v3 = vec<3>(scalar(row + 1), y3, scalar(col + 1));
        vec<3> v4 = vec<3>(scalar(row + 1), y4, scalar(col + 0));

        triangle<3> tri1(v1, v2, v4);
        triangle<3> tri2(v3, v4, v2);

        scalar trace_t1 = 2, trace_t2 = 2;
        vec<3> trace_p1, trace_p2;

        if (tri1.trace(ray<3>(v0, dv), trace_t1, trace_p1) || tri2.trace(ray<3>(v0, dv), trace_t2, trace_p2))
        {
            contact_info<3> ci;

            ci.happened = true;
            ci.penetrated = false;

            if (trace_t1 < trace_t2)
            {
                ci.position = trace_p1;
                ci.normal = tri1.get_normal();
				ci.time = trace_t1;
            }
            else
            {
                ci.position = trace_p2;
                ci.normal = tri2.get_normal();
				ci.time = trace_t2;
            }

            return ci;
        }
    }

    contact_info<3> ci;
    ci.happened = false;
    return ci;
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
    util::FileHandle(name.c_str(), O_RDONLY).read_all(buffer);

    if (buffer.size() != nheights() * sizeof(value_type))
        throw std::logic_error("heigtfield " + name + " contains invalid amount of data");

    load_from_buffer((value_type const *) &buffer[0]);
}

template class heightfield<boost::uint8_t>;
template class heightfield<boost::uint16_t>;

}
