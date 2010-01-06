//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#pragma once

#include <vector>
#include <math/vec.h>

namespace graphic
{

void build_smooth_normals(std::vector<math::vec<3> > const &positions, std::vector<unsigned> const &faces, std::vector<math::vec<3> > *normals);

}
