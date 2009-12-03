
#include <graphic/animated_mesh.h>
#include <graphic/animated_model.h>

namespace graphic
{

AnimatedModel::AnimatedModel(boost::shared_ptr<Renderer> const &renderer_ptr):
	Model(renderer_ptr)
{
}

AnimatedModel::~AnimatedModel()
{
}

MeshPtr AnimatedModel::add_mesh()
{
	meshes.push_back(MeshPtr(new AnimatedMesh(this)));
	return meshes.back();
}

}
