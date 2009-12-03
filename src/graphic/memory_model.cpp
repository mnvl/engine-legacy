
#include <graphic/renderer.h>
#include <graphic/memory_model.h>

namespace graphic
{

MemoryModel::MemoryModel(boost::shared_ptr<Renderer> const &renderer_ptr):
	Model(renderer_ptr)
{
}

MeshPtr MemoryModel::add_mesh()
{
	meshes.push_back(MeshPtr(new MemoryMesh(this)));
	return meshes.back();
}

void MemoryModel::draw()
{
	if (animations.size()) draw_animated();
	else draw_static();
}

void MemoryModel::draw_static()
{
	set_animation_time(0);

	for (joints_type::iterator it = joints.begin(); it != joints.end(); it++)
	{
		if (MemoryMeshPtr mesh_ptr = boost::static_pointer_cast<MemoryMesh>((*it)->mesh_ptr.lock()))
		{
			mesh_ptr->draw_static(it->get());
		}
	}
}

void MemoryModel::draw_animated()
{
	for (joints_type::iterator it = joints.begin(); it != joints.end(); it++)
	{
		if (MemoryMeshPtr mesh_ptr = boost::static_pointer_cast<MemoryMesh>((*it)->mesh_ptr.lock()))
		{
			mesh_ptr->draw_animated(it->get());
		}
	}
}

}
