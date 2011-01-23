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
#include <luabind/luabind.hpp>
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
    local_to_world_.scale(scale_.x / (ncols_ - 1), 1, scale_.z / (nrows_ - 1));
    local_to_world_.inverse(world_to_local_);

	heights_.resize(nrows * ncols);

	local_aabb_.lo.set(0, -1, 0);
	local_aabb_.hi.set(math::scalar(ncols_ - 1), 1, math::scalar(nrows_ - 1));
}

template<class T>
heightfield<T>::~heightfield()
{
}

template<class T>
vec<2, int> heightfield<T>::world_position_to_cell(vec<3> const &position)
{
	vec<3> p = position * world_to_local_;
	return vec<2, int>(int(p.x), int(p.z));
}

template<class T>
contact_info<3> heightfield<T>::trace(ray<3> const &r, scalar max_distance) const
{
	vec<3> local_p0 = r.r0 * world_to_local_;
	vec<3> local_p1 = (r.r0 + normalize(r.a) * max_distance) * world_to_local_;

	ray<3> local_ray(local_p0, local_p1 - local_p0);

	contact_info<3> result;
	result.happened = false;

	scalar local_ray_t0, local_ray_t1;

	if (!local_aabb_.trace(local_ray, &local_ray_t0, &local_ray_t1)) return result;

	vec<3> local_p0_adjusted = local_ray.apply(local_ray_t0);
	vec<3> local_p1_adjusted = local_ray.apply(local_ray_t1);

	bresenham_supercover<> cells_iterator(vec<2, int>(int (local_p0_adjusted.x), int(local_p0_adjusted.z)),
		vec<2, int>(int(local_p1_adjusted.x), int(local_p1_adjusted.z)));

	while (true)
	{
		vec<2, int> cell;
		bool flag;

		boost::tie(cell, flag) = cells_iterator.get();
		if (flag) break;

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

	result.position = result.position * local_to_world_;

	return result;
}

template<class T>
void heightfield<T>::load_from_raw_buffer(value_t const *heights)
{
    heights_.assign(heights, heights + nheights());

	post_load();
}

template<class T>
void heightfield<T>::resize(size_t ncols, size_t nrows)
{
	std::vector<value_t> heights;
	heights.resize(nrows * ncols);

	for (size_t i = 0, ni = std::min(nrows, nrows_); i < ni; ++i)
	{
		for (size_t j = 0, nj = std::min(ncols, ncols_); j < nj; ++j)
		{
			heights[i * ncols + j] = heights_[i * ncols_ + j];
		}
	}

	heights_.swap(heights);
	ncols_ = ncols;
	nrows_ = nrows;

	post_load();
}

template<class T>
void heightfield<T>::post_load()
{
	local_aabb_.null();

	for (std::vector<value_t>::const_iterator it = heights_.begin(); it != heights_.end(); ++it)
	{
		if (local_aabb_.lo.y > *it) local_aabb_.lo.y = *it;
		if (local_aabb_.hi.y < *it) local_aabb_.hi.y = *it;
	}

	if (local_aabb_.lo.y == local_aabb_.hi.y)
	{
		local_aabb_.lo.y -= 0.5f;
		local_aabb_.hi.y += 0.5f;
	}

	local_aabb_.lo.x = 0;
	local_aabb_.lo.z = 0;

	local_aabb_.hi.x = scalar(ncols_ - 1);
	local_aabb_.hi.z = scalar(nrows_ - 1);
}

template<class T>
void heightfield<T>::bind(lua_State *L, char const *name)
{
	using namespace luabind;

	module(L, "math")
	[
		class_<heightfield>(name)
		.def(constructor<matrix<4,4> const &, size_t, size_t, vec<3> const &>())
		.def("trace", &heightfield::trace)
		.def("ncols", &heightfield::nrows)
		.def("nrows", &heightfield::ncols)
		.def("resize", &heightfield::resize)
	];
}

template class heightfield<boost::uint8_t>;
template class heightfield<boost::uint16_t>;
template class heightfield<scalar>;

void bind_heightfield(lua_State *L)
{
	heightfield<boost::uint8_t>::bind(L, "heightfield_u8");
	heightfield<boost::uint16_t>::bind(L, "heightfield_u16");
	heightfield<scalar>::bind(L, "heightfield_s");
}

}
