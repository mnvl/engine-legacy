
#include <graphic/static_model.h>
#include <graphic/static_mesh.h>

namespace graphic
{

static_mesh::static_mesh(static_model *model_ptr):
mesh(model_ptr)
{
}

static_mesh::~static_mesh()
{
}

static_model *static_mesh::get_model()
{
	return (static_model *) model_ptr_;
}

void static_mesh::draw()
{
}

}
