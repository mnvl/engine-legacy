
#include <graphic/collada_loader.h>
#include <graphic/heightfield_loader.h>
#include <graphic/model.h>

namespace graphic {

model::model(renderer *renderer_ptr):
renderer_ptr_(renderer_ptr)
{
}

model::~model()
{
}

joint *model::create_joint()
{
	joints.push_back(new joint(joints.size()));
	return &joints.back();
}

void model::load_from_collada(std::string const &filename)
{
    collada_loader(filename, this, renderer_ptr_).load();
}

void load_from_raw_i16_heightfield(std::string const &filename, size_t nrows, size_t ncols,
    math::vec<3> const &scale, std::string const &texture_name)
{
    heightfield_loader(filename, this, renderer).load_raw<boost::int16_t>(nrows, ncols, scale, texture_name);
}

joint *model::get_joint_by_id(std::string const &id)
{
	for (joints_type::iterator it = joints.begin(); it != joints.end(); it++) {
		if (it->id == id) return &*it;
	}

	throw std::runtime_error("joint with id = " + id + " not found in model");
}

joint *model::get_joint_by_name(std::string const &name)
{
	for (joints_type::iterator it = joints.begin(); it != joints.end(); it++) {
		if (it->name == name) return &*it;
	}

	throw std::runtime_error("joint with name = " + name + " not found in model");
}

mesh *model::get_mesh_by_id(std::string const &id)
{
	for (meshes_type::iterator it = meshes.begin(); it != meshes.end(); it++) {
		if (it->id == id) return &*it;
	}

	throw std::runtime_error("mesh with id = " + id + " not found");
}

mesh *model::get_mesh_by_name(std::string const &name)
{
	for (meshes_type::iterator it = meshes.begin(); it != meshes.end(); it++) {
		if (it->name == name) return &*it;
	}

	throw std::runtime_error("mesh with name = " + name + " not found");
}

material *model::get_material_by_name(std::string const &name)
{
	for (materials_type::iterator it = materials.begin(); it != materials.end(); it++) {
		if (it->get_name() == name) return &*it;
	}

	throw std::runtime_error("material with name = " + name + " not found");
}

void model::set_animation_time(math::scalar t)
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


void model::draw()
{
	for (joints_type::iterator it = joints.begin(); it != joints.end(); it++)
	{
		if (it->mesh_ptr)
		{
			it->mesh_ptr->draw(&*it);
		}
	}
}

}
