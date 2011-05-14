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

#include <map>
#include <vector>
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
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
		virtual scalar get_score(vec<3> const &position, vec<3> const &goal) const = 0;
	};

	a_star(vec<2> const &start, vec<2> const &goal, scalar delta, scorer *scorer_ptr, scalar score_threshold = 0);
	~a_star();

	void calculate();

private:
	vec<2> const start_;
	node_t start_node_;
	vec<2> const goal_;
	node_t goal_node_;
	scalar const delta_;
	scalar const score_threshold_;
	scorer * const scorer_ptr_;

	class NodeHash: public std::unary_function<node_t, size_t> {
	public:
		size_t operator ()(node_t const &node) const
		{
			return node.x * 10133 + node.y;
		}
	};

	boost::unordered_set<node_t, NodeHash> closed_set_;
	std::multimap<scalar, node_t> open_set_forw_;
	boost::unordered_map<node_t, scalar, NodeHash> open_set_backw_;
	boost::unordered_map<node_t, node_t, NodeHash> came_from_;

	boost::unordered_map<node_t, scalar, NodeHash> g_score_; // distance from start along optimal path
	boost::unordered_map<node_t, scalar, NodeHash> f_score_; // heuristic estimate of distance
	boost::unordered_map<node_t, scalar, NodeHash> h_score_; // estimated total distance from start to goal through y

	void reconstruct_path();
};

}

