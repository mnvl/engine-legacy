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

#include "a_star.h"

namespace math
{

a_star::scorer::~scorer() {
}

a_star::a_star(vec<2> const &start, vec<2> const &goal, scalar delta, scorer *scorer_ptr, scalar score_threshold):
	start_(start),
	goal_(goal),
	delta_(delta),
	scorer_ptr_(scorer_ptr),
	score_threshold_(score_threshold)
{
	start_node_ = position_to_node(start_);
	goal_node_ = position_to_node(goal_);
}

a_star::~a_star()
{
}

void a_star::calculate()
{
	{
		scalar start_node_score = scorer_ptr_->get_score(start_, goal_);

		g_score_[start_node_] = 0;
		f_score_[start_node_] = h_score_[start_node_] = start_node_score;

		open_set_forw_.insert(std::make_pair(start_node_score, start_node_));
		open_set_backw_.insert(std::make_pair(start_node_, start_node_score));
	}

	while (!open_set_forw_.empty())
	{
		node_t current_node = open_set_forw_.begin()->second;

		if (current_node == goal_node_)
		{
			return reconstruct_path();
		}

		open_set_forw_.erase(open_set_forw_.begin());
		open_set_backw_.erase(current_node);

		closed_set_.insert(current_node);

		node_t neighbours[4];
		neighbours[0] = current_node + node_t(-1,  0);
		neighbours[1] = current_node + node_t(+1,  0);
		neighbours[2] = current_node + node_t( 0, +1);
		neighbours[3] = current_node + node_t( 0, +1);

		for (size_t i = 0; i < 4; ++i)
		{
			node_t const &next_node = neighbours[i];

			if (closed_set_.find(next_node) != closed_set_.end()) continue;

			scalar tentative_g_score = g_score_[current_node] + delta_;
			bool tentative_is_better;

			if (open_set_backw_.find(next_node) != open_set_backw_.end())
			{
				tentative_is_better = true;
			}
			else if (tentative_g_score < g_score_[next_node])
			{
				tentative_is_better = true;
			}
			else
			{
				tentative_is_better = false;
			}

			if (tentative_is_better)
			{
				came_from_[next_node] = current_node;

				scalar heuristic_score = scorer_ptr_->get_score(node_to_position(current_node), goal_);
				scalar f_score = heuristic_score + tentative_g_score;

				g_score_[next_node] = tentative_g_score;
				h_score_[next_node] = heuristic_score;
				f_score_[next_node] = f_score;

				if (open_set_backw_.find(next_node) != open_set_backw_.end())
				{
					scalar score = open_set_backw_[next_node];
				}

				open_set_backw_[next_node] = f_score;
			}
		}
	}
}

void a_star::reconstruct_path()
{
}

}
