
#include <engine/graphic/renderer.h>
#include <engine/graphic/memory_model.h>

namespace engine {
namespace graphic {

memory_mesh *memory_model::get_mesh_by_name(std::string const &name) {
	for (meshes_type::iterator it = meshes.begin(); it != meshes.end(); it++) {
		if (it->name == name) return &*it;
	}

	return 0;
}

model::joint *memory_model::get_joint_by_name(std::string const &name) {
	for (joints_type::iterator it = joints.begin(); it != joints.end(); it++) {
		if (it->name == name) return &*it;
	}

	throw std::runtime_error("joint " + name + " not found in model");
}

void memory_model::draw_wireframe(renderer *renderer_ptr, math::scalar time) const
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

	animation const *animation_ptr = &*animations.begin();

	for (animation::samplers_type::const_iterator it = animation_ptr->samplers.begin();
		it != animation_ptr->samplers.end(); it++)
	{
		if (it->get_joint()) {
			it->get_joint()->frame_transform = it->get(time);
		}
	}

	for (meshes_type::const_iterator it = meshes.begin(); it != meshes.end(); it++) {
		it->draw_wireframe(renderer_ptr, time);
	}
}

}
}