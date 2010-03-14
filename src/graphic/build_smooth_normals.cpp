//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include "build_smooth_normals.h"

namespace graphic
{

void build_smooth_normals(std::vector<GenericVertex> &vertices, std::vector<unsigned> const &faces)
{
    for (std::vector<GenericVertex>::iterator it = vertices.begin(); it != vertices.end(); ++it)
	{
		it->normal.set(0, 0, 0);
	}

    std::vector<math::scalar> adjacent_faces_count(vertices.size(), 0.0f);

    for (std::vector<unsigned>::const_iterator it = faces.begin(); it != faces.end(); it += 3)
    {
		math::vec<3> face_normal = (vertices[*(it + 1)].position - vertices[*(it + 0)].position) ^
			(vertices[*(it + 2)].position - vertices[*(it + 1)].position);

		vertices[*(it + 0)].normal += face_normal;
        adjacent_faces_count[*(it + 0)]++;

		vertices[*(it + 1)].normal += face_normal;
        adjacent_faces_count[*(it + 1)]++;

		vertices[*(it + 2)].normal += face_normal;
        adjacent_faces_count[*(it + 1)]++;
    }

    for (size_t i = 0; i < vertices.size(); i++)
    {
		vertices[i].normal /= adjacent_faces_count[i];
    }
}

}
