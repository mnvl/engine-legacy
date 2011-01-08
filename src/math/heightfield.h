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
#include "vec.h"
#include "line.h"
#include "matrix.h"
#include "collision.h"

namespace math
{

template<class T = scalar>
class heightfield {
public:
    typedef T value_type;

    heightfield(matrix<4,4> const &tf, size_t ncols, size_t nrows, vec<3> const &scale);
    ~heightfield();

	scalar local_y_at(int col, int row) const { return heights_.at(row * ncols_ + col); }
	scalar y_at(int col, int row) const { return local_y_at(col, row) * scale_.y; }
	vec<3> local_vertex_at(int col, int row) { return vec<3>(scalar(col), local_y_at(col, row), scalar(row)); }

	size_t ncols() const { return ncols_; }
	size_t nrows() const { return nrows_; }

    contact_info<3> trace(ray<3> const &r, scalar max_distance = 1000.0f);

    void load_from_buffer(value_type const *heights);
    void load_from_raw_file(std::string const &name);

private:
    matrix<4,4> local_to_world_, world_to_local_;
    size_t ncols_, nrows_;
    vec<3> scale_;
    std::vector<value_type> heights_;

    size_t nheights()
    {
        return nrows_ * ncols_;
    }
};

}
