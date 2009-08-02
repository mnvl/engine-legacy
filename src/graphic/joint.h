#pragma once

#include <string>
#include <vector>
#include <math/matrix.h>

namespace graphic {

class Mesh;

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

	size_t index;
	std::string id, name;
	Joint *parent;
	std::vector<Joint *> childs;
	std::vector<transform> transforms;
	mutable math::matrix<4,4> matrix;
	Mesh *mesh_ptr;

	Joint(size_t i):
	index(i),
	parent(0),
	mesh_ptr(0)
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

		if (parent) matrix *= parent->matrix;

		for (std::vector<Joint *>::iterator it = childs.begin(); it != childs.end(); it++)
		{
			(*it)->build_frame_transforms();
		}
	}
};

}

