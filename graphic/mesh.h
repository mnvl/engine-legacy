#pragma once

#include <list>
#include <boost/shared_ptr.hpp>
#include <graphic/joint_influence.h>
#include <graphic/material.h>

namespace graphic {

class model;

class mesh {
public:
	struct assigned_material
	{
		material *material_ptr;
		size_t triangles_count;
	};

	typedef std::list<joint_influence> influences_type;
	typedef std::vector<assigned_material> assigned_materials_type;

	std::string id, name;
	assigned_materials_type assigned_materials;
	influences_type influences;

	mesh(model *model_ptr = 0);
	virtual ~mesh();

	joint_influence *get_joint_influence_by_name(std::string const &name);

	virtual model *get_model() const;
	virtual void draw();

protected:
	model *model_ptr_;
};

}

