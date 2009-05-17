
#include <graphic/animated_mesh.h>
#include <graphic/animated_model.h>

namespace graphic
{

animated_model::animated_model(renderer *renderer_ptr):
model(renderer_ptr)
{
}

animated_model::~animated_model()
{
}

mesh *animated_model::create_mesh()
{
	std::auto_ptr<animated_mesh> mesh_ptr(new animated_mesh(this));
	meshes.push_back(mesh_ptr);
	return &meshes.back();
}

}
