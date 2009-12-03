
#include <boost/lambda/lambda.hpp>
#include <exception/directx_error.h>
#include <graphic/static_model.h>

namespace graphic
{

StaticModel::StaticModel(boost::shared_ptr<Renderer> const &renderer_ptr):
Model(renderer_ptr)
{
}

StaticModel::~StaticModel()
{
}

MeshPtr StaticModel::add_mesh()
{
	meshes.push_back(MeshPtr(new StaticMesh(this)));
	return meshes.back();
}

}
