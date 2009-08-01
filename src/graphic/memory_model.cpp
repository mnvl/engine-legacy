
#include <graphic/renderer.h>
#include <graphic/memory_model.h>

namespace graphic
{

memory_model::memory_model(renderer *renderer_ptr):
model(renderer_ptr)
{
}

mesh *memory_model::create_mesh()
{
	std::auto_ptr<memory_mesh> mesh_ptr(new memory_mesh(this));
	meshes.push_back(mesh_ptr);
	return &meshes.back();
}

void memory_model::draw()
{
	if (true || animations.size()) draw_animated();
	else draw_static();
}

void memory_model::draw_static()
{
	set_animation_time(0);

	for (joints_type::iterator it = joints.begin(); it != joints.end(); it++)
	{
		if (it->mesh_ptr)
		{
			((memory_mesh *) it->mesh_ptr)->draw_static(&*it);
		}
	}
}

void memory_model::draw_animated()
{
	for (joints_type::iterator it = joints.begin(); it != joints.end(); it++)
	{
		if (it->mesh_ptr)
		{
			((memory_mesh *) it->mesh_ptr)->draw_animated(&*it);
		}
	}
}

}
