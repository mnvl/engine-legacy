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

a_star::a_star(vec<2> const &start, vec<2> const &goal, scalar delta, scorer *scorer_ptr, scalar negative_score_threshold):
	start_(start),
	goal_(goal),
	delta_(delta),
	scorer_ptr_(scorer_ptr),
	negative_score_threshold_(negative_score_threshold)
{
}

a_star::~a_star()
{
}

}
