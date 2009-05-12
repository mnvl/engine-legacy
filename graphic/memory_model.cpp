
#include <graphic/renderer.h>
#include <graphic/memory_model.h>

namespace graphic
{

memory_model::memory_model(renderer *renderer_ptr):
model(renderer_ptr)
{
}

void memory_model::cleanup()
{
	meshes_type::iterator it = meshes.begin();
	while (it != meshes.end()) {
		if (((memory_mesh *) &*it)->indices.empty()) it = meshes.erase(it);
		else it++;
	}
}

void memory_model::set_animation_time(math::scalar t)
{
	for (animations_type::iterator it = animations.begin(); it != animations.end(); it++)
	{
		it->set_time(t);
	}

	for (joints_type::iterator it = joints.begin(); it != joints.end(); it++) {
		if (it->parent == 0) {
			it->build_frame_transforms();
		}
	}
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

}
