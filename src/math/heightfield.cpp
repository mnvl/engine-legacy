//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2011 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <stdexcept>
#include <utility>
#include <boost/cstdint.hpp>
#include <util/file_handle.h>
#include "bresenham_supercover.h"
#include "heightfield.h"

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

	heights_.resize(nrows * ncols);
}

template<class T>
heightfield<T>::~heightfield()
{
}

template<class T>
contact_info<3> heightfield<T>::trace(ray<3> const &r, scalar max_distance)
{
	vec<3> local_p0 = r.r0 * world_to_local_;
	vec<3> local_p1 = (r.r0 + r.a * max_distance) * world_to_local_;

	bresenham_supercover<> cells_iterator(vec<2, int>(int (local_p0.x), int(local_p0.z)),
		vec<2, int>(int(local_p1.x), int(local_p1.z)));

	ray<3> local_ray(local_p0, local_p1 - local_p0);

	contact_info<3> result;
	result.happened = false;

	while (true)
	{
		vec<2, int> cell;
		bool flag;

		boost::tie(cell, flag) = cells_iterator.get();
		if (flag) break;

		if (cell.x < 0 || cell.y < 0 || cell.x >= ncols_ || cell.y >= nrows_) continue;

		if (cell.x == ncols_ - 1) --cell.x;
		if (cell.y == nrows_ - 1) --cell.y;

		vec<3> v1 = local_vertex_at(cell.x + 0, cell.y + 0);
		vec<3> v2 = local_vertex_at(cell.x + 0, cell.y + 1);
		vec<3> v3 = local_vertex_at(cell.x + 1, cell.y + 1);
		vec<3> v4 = local_vertex_at(cell.x + 1, cell.y + 0);

        triangle<3> tri1(v1, v2, v3);
        triangle<3> tri2(v1, v3, v4);

		bool trace_h1, trace_h2;
        scalar trace_t1, trace_t2;
		vec<3> trace_p1, trace_p2;

		trace_h1 = tri1.trace(local_ray, trace_t1, trace_p1);
		trace_h2 = tri2.trace(local_ray, trace_t2, trace_p2);

		if (trace_h1 || trace_h2)
        {
            contact_info<3> ci;

            ci.happened = true;
            ci.penetrated = false;

            if (trace_h1 && ((trace_h2 && trace_t1 < trace_t2) || trace_h2 == false))
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

			if (result.worse_than(ci))
			{
				result = ci;
			}
        }
    }

	return result;
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
template class heightfield<scalar>;

}
