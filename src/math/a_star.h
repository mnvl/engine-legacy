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

#include "vec.h"

namespace math
{

class a_star {
public:
	typedef vec<2, int> node_t;

	node_t const position_to_node(vec<2> const &position) {
		vec<2> delta = (position - start_) / delta_;
		return vec<2, int>(int(delta.x), int(delta.y));
	}

	vec<2> const node_to_position(node_t const &node)
	{
		return start_ + vec<2>(node.x * delta_, node.y * delta_);
	}

	class scorer {
	public:
		virtual ~scorer();
		virtual scalar get_score(vec<3> const &position) const = 0;
	};

	a_star(vec<2> const &start, vec<2> const &goal, scalar delta, scorer *scorer_ptr, scalar negative_score_threshold = 0);
	~a_star();

private:
	vec<2> const start_;
	vec<2> const goal_;
	scalar const delta_;
	scalar const negative_score_threshold_;
	scorer * const scorer_ptr_;
};

}

