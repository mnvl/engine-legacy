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

#include <maya/MQuaternion.h>
#include <math/vec.h>
#include <math/quaternion.h>

inline math::vec<4> const
convert(const MColor &color)
{
	return math::vec<4>(color.r, color.g, color.b, color.a);
}

inline math::vec<3> convert(const MFloatVector &vector) {
        math::vec<3> v;
        v.x = vector.x;
        v.y = vector.y;
        v.z = vector.z;
        return v;
}

inline math::vec<3> convert(const MVector &vector) {
        math::vec<3> v;
        v.x = math::scalar(vector.x);
        v.y = math::scalar(vector.y);
        v.z = math::scalar(vector.z);
        return v;
}

inline math::vec<3> convert(const MPoint &vector) {
        math::vec<3> v;
        v.x = math::scalar(vector.x);
        v.y = math::scalar(vector.y);
        v.z = math::scalar(vector.z);
        return v;
}

inline math::quaternion<> convert(const MQuaternion &quat) {
        math::quaternion<> q;
        q.x = math::scalar(quat.x);
        q.y = math::scalar(quat.y);
        q.z = math::scalar(quat.z);
        q.w = math::scalar(quat.w);
        return q;
}
