
#include <boost/lambda/lambda.hpp>
#include <exception/directx_error.h>
#include <graphic/static_model.h>

namespace graphic
{

StaticModel::StaticModel(Renderer *renderer_ptr):
Model(renderer_ptr)
{
}

StaticModel::~StaticModel()
{
}

Mesh *StaticModel::create_mesh()
{
	std::auto_ptr<StaticMesh> mesh_ptr(new StaticMesh(this));
	meshes.push_back(mesh_ptr);
	return &meshes.back();
}

}
