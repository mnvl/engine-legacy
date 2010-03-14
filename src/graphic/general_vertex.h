#pragma once
//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <math/vec.h>

namespace graphic
{

struct GeneralVertex {
	struct Less
	{
		inline bool operator ()(GeneralVertex const &lhs, GeneralVertex const &rhs) const
		{
			if ((lhs.position - rhs.position).length_sq() > math::EPSILON)
			{
				if (math::lexicographical_less(lhs.position, rhs.position)) return true;
				if (math::lexicographical_greater(lhs.position, rhs.position)) return false;
			}

			if ((lhs.tangent - rhs.tangent).length_sq() > math::EPSILON)
			{
				if (math::lexicographical_less(lhs.tangent, rhs.tangent)) return true;
				if (math::lexicographical_greater(lhs.tangent, rhs.tangent)) return false;
			}

			if ((lhs.binormal - rhs.binormal).length_sq() > math::EPSILON)
			{
				if (math::lexicographical_less(lhs.binormal, rhs.binormal)) return true;
				if (math::lexicographical_greater(lhs.binormal, rhs.binormal)) return false;
			}

			if ((lhs.normal - rhs.normal).length_sq() > math::EPSILON)
			{
				if (math::lexicographical_less(lhs.normal, rhs.normal)) return true;
				if (math::lexicographical_greater(lhs.normal, rhs.normal)) return false;
			}

			if ((lhs.texcoords - rhs.texcoords).length_sq() > math::EPSILON)
			{
				if (math::lexicographical_less(lhs.texcoords, rhs.texcoords)) return true;
			}

			return false;
		}
	};

	math::vec<3> position;
	math::vec<3> tangent;
	math::vec<3> binormal;
	math::vec<3> normal;
	math::vec<2> texcoords;
};

}

