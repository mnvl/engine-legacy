
#include <boost/lambda/lambda.hpp>
#include <exception/directx_error.h>
#include <graphic/static_model.h>

namespace graphic {

static_model::static_model(renderer *renderer_ptr):
model(renderer_ptr)
{
}

static_model::~static_model()
{
}

mesh *static_model::create_mesh()
{
	std::auto_ptr<static_mesh> mesh_ptr(new static_mesh(this));
	meshes.push_back(mesh_ptr);
	return &meshes.back();
}

}
