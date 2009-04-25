
#include <engine/graphic/renderer.h>
#include <engine/graphic/memory_model.h>

namespace engine {
namespace graphic {

void memory_model::cleanup()
{
	meshes_type::iterator it = meshes.begin();
	while (it != meshes.end()) {
		if (it->indices.empty()) it = meshes.erase(it);
		else it++;
	}
}

memory_mesh *memory_model::get_mesh_by_id(std::string const &id)
{
	for (meshes_type::iterator it = meshes.begin(); it != meshes.end(); it++) {
		if (it->id == id) return &*it;
	}

	throw std::runtime_error("mesh with id = " + id + " not found");
}

memory_mesh *memory_model::get_mesh_by_name(std::string const &name, bool can_create)
{
	for (meshes_type::iterator it = meshes.begin(); it != meshes.end(); it++) {
		if (it->name == name) return &*it;
	}

	if (can_create) {
		memory_mesh mm(this);
		mm.name = name;
		meshes.push_back(mm);
		return &meshes.back();
	}

	throw std::runtime_error("mesh with name = " + name + " not found");
}

model::joint *memory_model::get_joint_by_id(std::string const &id)
{
	for (joints_type::iterator it = joints.begin(); it != joints.end(); it++) {
		if (it->id == id) return &*it;
	}

	throw std::runtime_error("joint with id = " + id + " not found in model");
}

model::joint *memory_model::get_joint_by_name(std::string const &name)
{
	for (joints_type::iterator it = joints.begin(); it != joints.end(); it++) {
		if (it->name == name) return &*it;
	}

	throw std::runtime_error("joint with name = " + name + " not found in model");
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

	if (time > 0)
	{
		animation const *animation_ptr = &*animations.begin();

		for (animation::samplers_type::const_iterator it = animation_ptr->samplers.begin();
			it != animation_ptr->samplers.end(); it++)
		{
			if (it->get_joint()) {
				it->get_joint()->interpolated_transform = it->get(time);
			}
		}
	}

	for (meshes_type::const_iterator it = meshes.begin(); it != meshes.end(); it++) {
		it->draw_wireframe(renderer_ptr, time);
	}
}

}
}