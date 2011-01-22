#pragma once
//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2011 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <vector>
#include <luabind/lua_include.hpp>
#include <boost/shared_ptr.hpp>
#include "vec.h"
#include "aabb.h"
#include "line.h"
#include "matrix.h"
#include "collision.h"

namespace math
{

template<class T = scalar>
class heightfield {
public:
    typedef T value_t;

    heightfield(matrix<4,4> const &tf, size_t ncols, size_t nrows, vec<3> const &scale);
    ~heightfield();

	value_t local_y_at(int col, int row) const { return heights_.at(row * ncols_ + col); }
	value_t &local_y_at(int col, int row) { return heights_.at(row * ncols_ + col); }

	scalar y_at(int col, int row) const { return local_y_at(col, row) * scale_.y; }
	vec<3> local_vertex_at(int col, int row) const { return vec<3>(scalar(col), local_y_at(col, row), scalar(row)); }

	size_t ncols() const { return ncols_; }
	size_t nrows() const { return nrows_; }

	vec<2, int> world_position_to_cell(vec<3> const &position);

    contact_info<3> trace(ray<3> const &r, scalar max_distance = 1000.0f) const;

    void load_from_raw_buffer(value_t const *heights);

	template<class Archive>
	void serialize(Archive &archive, unsigned const /*file_version*/)
	{
		archive & local_to_world_ & world_to_local_ & ncols_ & nrows_ & heights_ & local_aabb_;
	}

	static void bind(lua_State *L, char const *name);

private:
    matrix<4,4> local_to_world_, world_to_local_;
    size_t ncols_, nrows_;
    vec<3> scale_;
    std::vector<value_t> heights_;
	aabb<3> local_aabb_;

    size_t nheights()
    {
        return nrows_ * ncols_;
    }

	void post_load();
};

void bind_heightfield(lua_State *L);

}
