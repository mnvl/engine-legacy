//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <algorithm>
#include <graphic/build_smooth_normals.h>

namespace graphic
{

void build_smooth_normals(std::vector<math::vec<3> > const &positions, std::vector<unsigned> const &faces, std::vector<math::vec<3> > *normals)
{
    normals->assign(positions.size(), math::vec<3>(0, 0, 0));
    std::vector<math::scalar> adjacent_faces_count(positions.size(), 0.0f);

    for (std::vector<unsigned>::const_iterator it = faces.begin(); it != faces.end(); it += 3)
    {
        math::vec<3> face_normal = (positions[*(it + 1)] - positions[*(it + 0)]) ^ (positions[*(it + 2)] - positions[*(it + 1)]);

        (*normals)[*(it + 0)] += face_normal;
        adjacent_faces_count[*(it + 0)]++;

        (*normals)[*(it + 1)] += face_normal;
        adjacent_faces_count[*(it + 1)]++;

        (*normals)[*(it + 2)] += face_normal;
        adjacent_faces_count[*(it + 1)]++;
    }

    for (size_t i = 0; i < positions.size(); i++)
    {
		(*normals)[i] /= adjacent_faces_count[i];
    }
}

}
