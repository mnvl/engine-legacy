
#include <graphic/model.h>
#include <graphic/mesh.h>

namespace graphic {

Mesh::Mesh(Model *model_ptr):
model_ptr_(model_ptr)
{
}

Mesh::~Mesh()
{
}

void Mesh::set_model(Model *model)
{
	model_ptr_ = model;
}

Model *Mesh::get_model() const
{
	return model_ptr_;
}

void Mesh::draw(Joint *joint_ptr)
{
}

}


