
#include <graphic/renderer.h>
#include <graphic/memory_model.h>

namespace graphic
{

MemoryModel::MemoryModel(boost::shared_ptr<Renderer> const &renderer_ptr):
	Model(renderer_ptr)
{
}

Mesh *MemoryModel::create_mesh()
{
	std::auto_ptr<MemoryMesh> mesh_ptr(new MemoryMesh(this));
	meshes.push_back(mesh_ptr);
	return &meshes.back();
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
		if (it->mesh_ptr)
		{
			((MemoryMesh *) it->mesh_ptr)->draw_static(&*it);
		}
	}
}

void MemoryModel::draw_animated()
{
	for (joints_type::iterator it = joints.begin(); it != joints.end(); it++)
	{
		if (it->mesh_ptr)
		{
			((MemoryMesh *) it->mesh_ptr)->draw_animated(&*it);
		}
	}
}

}
