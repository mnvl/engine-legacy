//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2011 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include "aabb.h"
#include "obb.h"
#include "frustum.h"

namespace math
{

template<class T>
frustum<T>::frustum()
{
}

template<class T>
frustum<T>::frustum(matrix<4,4> const &tf)
{
	// http://zach.in.tu-clausthal.de/teaching/cg_literatur/lighthouse3d_view_frustum_culling/index.html

	// left clipping plane
	planes[0].A = tf._14 + tf._11;
	planes[0].B = tf._24 + tf._21;
	planes[0].C = tf._34 + tf._31;
	planes[0].D = tf._44 + tf._41;

	// right clipping plane
	planes[1].A = tf._14 - tf._11;
	planes[1].B = tf._24 - tf._21;
	planes[1].C = tf._34 - tf._31;
	planes[1].D = tf._44 - tf._41;

	// top clipping plane
	planes[2].A = tf._14 - tf._12;
	planes[2].B = tf._24 - tf._22;
	planes[2].C = tf._34 - tf._32;
	planes[2].D = tf._44 - tf._42;

	// bottom clipping plane
	planes[3].A = tf._14 + tf._12;
	planes[3].B = tf._24 + tf._22;
	planes[3].C = tf._34 + tf._32;
	planes[3].D = tf._44 + tf._42;

	// near clipping plane
	planes[4].A = tf._13;
	planes[4].B = tf._23;
	planes[4].C = tf._33;
	planes[4].D = tf._43;

	// far clipping plane
	planes[5].A = tf._14 - tf._13;
	planes[5].B = tf._24 - tf._23;
	planes[5].C = tf._34 - tf._33;
	planes[5].D = tf._44 - tf._43;
}

template<class T>
frustum<T>::~frustum()
{
}

template<class T>
bool frustum<T>::contains(math::vec<3> const &point) const
{
	for(size_t i = 0; i < PLANES_COUNT; i++)
	{
		if (planes[i].classify(point) < 0) return false;
	}

	return true;
}

template<class T>
bool frustum<T>::test_intersection(aabb<3,T> const &bounds) const
{
	for (size_t i = 0; i < PLANES_COUNT; ++i)
	{
		if (planes[i].classify(bounds) == plane_t::NEGATIVE) return false;
	}

	return true;
}

template<class T>
bool frustum<T>::test_intersection(obb<3,T> const &bounds) const
{
	for (size_t i = 0; i < PLANES_COUNT; ++i)
	{
		if (planes[i].classify(bounds) == plane_t::NEGATIVE) return false;
	}

	return true;
}

template class frustum<>;

}
