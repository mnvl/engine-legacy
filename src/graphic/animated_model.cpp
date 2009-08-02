
#include <graphic/animated_mesh.h>
#include <graphic/animated_model.h>

namespace graphic
{

AnimatedModel::AnimatedModel(Renderer *renderer_ptr):
Model(renderer_ptr)
{
}

AnimatedModel::~AnimatedModel()
{
}

Mesh *AnimatedModel::create_mesh()
{
	std::auto_ptr<AnimatedMesh> mesh_ptr(new AnimatedMesh(this));
	meshes.push_back(mesh_ptr);
	return &meshes.back();
}

}
