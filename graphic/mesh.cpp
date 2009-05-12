
#include <graphic/model.h>
#include <graphic/mesh.h>

namespace graphic {

mesh::mesh(model *model_ptr):
model_ptr_(model_ptr)
{
}

mesh::~mesh()
{
}

joint_influence *mesh::get_joint_influence_by_name(std::string const &name)
{
	joint *joint_ptr = get_model()->get_joint_by_name(name);

	for (influences_type::iterator it = influences.begin(); it != influences.end(); it++)
	{
		if (it->joint_ptr == joint_ptr) return &*it;
	}

	throw std::runtime_error("influence of joint " + joint_ptr->name + " to mesh " + name + " not found");
}


model *mesh::get_model() const
{
	return model_ptr_;
}

void mesh::draw()
{
}

}


