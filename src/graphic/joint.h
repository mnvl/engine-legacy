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

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <lua.h>
#include <math/matrix.h>

namespace graphic {

class Joint;
class Mesh;

typedef boost::shared_ptr<Joint> JointPtr;
typedef boost::weak_ptr<Joint> JointWeakPtr;

class Joint {
public:
	struct transform {
		transform(std::string const &s, math::matrix<4,4> const &m):
		sid(s), matrix(m)
		{
		}

		std::string sid;
		math::matrix<4,4> matrix;
	};

	typedef std::vector<JointWeakPtr> childs_type;

	size_t index;
	std::string id, name;
	JointWeakPtr parent;
	childs_type childs;
	std::vector<transform> transforms;
	mutable math::matrix<4,4> matrix;
	boost::weak_ptr<Mesh> mesh_ptr;

	Joint(size_t i):
	index(i)
	{
	}

	math::matrix<4,4> &get_transform_by_sid(std::string const &sid){
		for (std::vector<transform>::iterator it = transforms.begin(); it != transforms.end(); it++)
			if (it->sid == sid) return it->matrix;
		throw std::logic_error("transform sid = " + sid + " not found");
	}

	void build_frame_transforms()
	{
		matrix.identity();

		for (std::vector<transform>::reverse_iterator it = transforms.rbegin();
			it != transforms.rend(); it++)
		{
			matrix *= it->matrix;
		}

		if (JointPtr p = parent.lock()) matrix *= p->matrix;

		for (childs_type::iterator it = childs.begin(); it != childs.end(); it++)
		{
			if (JointPtr child = it->lock()) child->build_frame_transforms();
		}
	}

	static void bind(lua_State *L);
};

}

