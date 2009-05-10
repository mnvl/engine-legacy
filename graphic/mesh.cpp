
#include <graphic/mesh.h>

namespace engine {
namespace graphic {

mesh::mesh(model *model_ptr):
model_ptr_(model_ptr)
{
}

mesh::~mesh()
{
}

void mesh::set_material(boost::shared_ptr<material> const &material_ptr)
{
	material_ptr_ = material_ptr;
}

model *mesh::get_model() const
{
	return model_ptr_;
}

void mesh::draw()
{
}

}
}

