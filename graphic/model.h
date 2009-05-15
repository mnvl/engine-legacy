#pragma once

#include <boost/ptr_container/ptr_vector.hpp>
#include <graphic/joint.h>
#include <graphic/animation.h>
#include <graphic/mesh.h>

namespace graphic {

class model {
public:
	typedef boost::ptr_vector<joint> joints_type;
	typedef boost::ptr_vector<animation> animations_type;
	typedef boost::ptr_vector<mesh> meshes_type;
	typedef boost::ptr_vector<material> materials_type;

	joints_type joints;
	meshes_type meshes;
	animations_type animations;
	materials_type materials;

	model(renderer *renderer_ptr = 0);
	virtual ~model();

	renderer *get_renderer() const
	{
		return renderer_ptr_;
	}

	joint *get_joint_by_id(std::string const &name);
	joint *get_joint_by_name(std::string const &name);
	mesh *get_mesh_by_id(std::string const &id);
	mesh *get_mesh_by_name(std::string const &name);
	material *get_material_by_name(std::string const &name);

	virtual joint *create_joint();
	virtual mesh *create_mesh() = 0;

protected:
	renderer *renderer_ptr_;
};

}
