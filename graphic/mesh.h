#pragma once

#include <boost/shared_ptr.hpp>
#include <graphic/joint_influence.h>
#include <graphic/material.h>

namespace graphic {

class model;

class mesh {
public:
	std::string id, name;

	mesh(model *model_ptr = 0);
	virtual ~mesh();

	virtual void set_material(boost::shared_ptr<material> const &material_ptr);
	virtual model *get_model() const;
	virtual void draw();

protected:
	model *model_ptr_;
	boost::shared_ptr<material> material_ptr_;
};

}

