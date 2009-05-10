
#include <graphic/renderer.h>
#include <graphic/memory_model.h>

namespace graphic {

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

void memory_model::draw_wireframe(renderer *renderer_ptr, math::scalar time)
{
	D3DMATERIAL9 material;

	material.Ambient.r = 0.0f;
	material.Ambient.g = 1.0f;
	material.Ambient.b = 0.0f;
	material.Ambient.a = 1.0f;

	material.Diffuse = material.Ambient;
	material.Specular = material.Ambient;
	material.Emissive = material.Ambient;
	material.Power = 1.0f;

	renderer_ptr->get_device()->SetMaterial(&material);
	renderer_ptr->get_device()->SetFVF(memory_mesh::VERTEX_FORMAT);

	set_animation_time(time);

	for (joints_type::iterator it = joints.begin(); it != joints.end(); it++)
	{
		if (it->mesh_ptr)
		{
			((memory_mesh *) it->mesh_ptr)->draw_wireframe(renderer_ptr, &*it, time);
		}
	}
}

}
