
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

model *mesh::get_model() const
{
	return model_ptr_;
}

void mesh::draw()
{
}

}


