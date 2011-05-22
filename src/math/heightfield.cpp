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
#include <boost/bind.hpp>
#include <luabind/luabind.hpp>
#include <luabind/iterator_policy.hpp>
#include <math/a_star.h>
#include "bresenham_supercover.h"
#include "heightfield.h"

namespace math
{

template<class T>
heightfield<T>::heightfield(matrix<4,4> const &tf, size_t nrows, size_t ncols):
    nrows_(nrows),
    ncols_(ncols)
{
	set_local_to_world(tf);

	heights_.resize(nrows * ncols);

	local_aabb_.lo.set(0, -1, 0);
	local_aabb_.hi.set(math::scalar(ncols_ - 1), 1, math::scalar(nrows_ - 1));
}

template<class T>
heightfield<T>::~heightfield()
{
}

template<class T>
scalar heightfield<T>::y_under(vec<3> const &position, scalar default_value)
{
	vec<3> local_position = position * world_to_local_;
	if (local_position.x < 0 || local_position.y < 0) return default_value;

	vec<2, unsigned> cell(unsigned(local_position.x), unsigned(local_position.z));
	if (cell.x >= ncols_ - 1 || cell.y >= nrows_ - 1) return default_value;

	scalar v1 = local_y_at(cell.x + 0, cell.y + 0);
	scalar v2 = local_y_at(cell.x + 1, cell.y + 0);
	scalar v3 = local_y_at(cell.x + 0, cell.y + 1);
	scalar v4 = local_y_at(cell.x + 1, cell.y + 1);

	scalar k1 = local_position.x - cell.x;
	scalar k2 = local_position.z - cell.x;

	scalar v12 = v1 * (1 - k1) + v2 * k1;
	scalar v34 = v3 * (1 - k1) + v4 * k1;

	return v12 * (1 - k2) + v34 * k2;
}

template<class T>
void heightfield<T>::set_local_to_world(math::matrix<4, 4> const &tf)
{
	local_to_world_ = tf;
    local_to_world_.inverse(world_to_local_);
}

template<class T>
vec<2, int> heightfield<T>::world_position_to_cell(vec<3> const &position)
{
	vec<3> p = position * world_to_local_;
	return vec<2, int>(int(p.x), int(p.z));
}

template<class T>
vec<3> heightfield<T>::cell_to_world_position(vec<2, int> const &cell)
{
	return vec<3>((scalar) cell.x, 0, (scalar) cell.y) * local_to_world_;
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

		triangle<3> t1(v1, v2, v3);
		triangle<3> t2(v1, v3, v4);

		bool trace_h1, trace_h2;
        scalar trace_t1, trace_t2;
		vec<3> trace_p1, trace_p2;

		trace_h1 = t1.trace(local_ray, trace_t1, trace_p1);
		trace_h2 = t2.trace(local_ray, trace_t2, trace_p2);

		if (trace_h1 || trace_h2)
        {
            contact_info<3> ci;

            ci.happened = true;
            ci.penetrated = false;

            if (trace_h1 && ((trace_h2 && trace_t1 < trace_t2) || trace_h2 == false))
            {
                ci.position = trace_p1;
                ci.normal = t1.get_normal();
                ci.time = trace_t1;
            }
            else
            {
                ci.position = trace_p2;
                ci.normal = t2.get_normal();
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
    heights_.assign(heights, heights + nsamples());

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
std::vector< vec<2, int> > heightfield<T>::build_path(vec<3> const &from, vec<3> const &to, value_t min_value, bool allow_best_heuristic_point)
{
	vec<2, int> start = world_position_to_cell(from);
	vec<2, int> goal = world_position_to_cell(to);

	struct scorer
	{
		heightfield const *hf;
		scalar min_value;
		vec<2, int> goal;

		scalar get_score(vec<2, int> const &cell)
		{
			scalar delta = (scalar) (cell - goal).length_sq();

			if (cell.x < 0) return -1;
			if (cell.y < 0) return -1;
			if (cell.x >= (int) hf->nrows()) return -1;
			if (cell.y >= (int) hf->ncols()) return -1;

			if (hf->local_y_at(cell.x, cell.y) < min_value)
			{
				return delta;
			}
			else
			{
				return -1;
			}
		}
	};

	scorer s;
	s.hf = this;
	s.min_value = min_value;
	s.goal = goal;

	a_star pathfinder(start, goal, boost::bind(&scorer::get_score, &s, _1));
	pathfinder.calculate_path();

	return pathfinder.build_path(allow_best_heuristic_point);
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

	local_aabb_.lo.y -= 1;
	local_aabb_.hi.y += 1;

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
		.def(constructor<matrix<4,4> const &, size_t, size_t>())
		.def("y_under", &heightfield::y_under)
		.def("ncols", &heightfield::ncols)
		.def("nrows", &heightfield::nrows)
		.def("set_local_to_world", &heightfield::set_local_to_world)
		.def("get_local_aabb", &heightfield::get_local_aabb)
		.def("world_position_to_cell", &heightfield::world_position_to_cell)
		.def("cell_to_world_position", &heightfield::cell_to_world_position)
		.def("build_path", &heightfield::build_path)
		.def("trace", &heightfield::trace)
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
