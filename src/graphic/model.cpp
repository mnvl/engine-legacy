
#include <graphic/collada_loader.h>
#include <graphic/heightfield_loader.h>
#include <graphic/model.h>

namespace graphic {

Model::Model(Renderer *renderer_ptr):
renderer_ptr_(renderer_ptr)
{
}

Model::~Model()
{
}

Joint *Model::create_joint()
{
	joints.push_back(new Joint(joints.size()));
	return &joints.back();
}

void Model::load_from_collada(std::string const &filename)
{
    ColladaLoader(filename, this, renderer_ptr_).load();
}

void Model::load_from_raw_i16_heightfield(std::string const &filename, math::matrix<4,4> const &tf,
	size_t nrows, size_t ncols, math::vec<3> const &scale, std::string const &texture_name)
{
    HeightfieldLoader(filename, this, renderer_ptr_).load_from_raw<boost::int16_t>(tf, nrows, ncols, scale, texture_name);
}

void Model::load_from_raw_u16_heightfield(std::string const &filename, math::matrix<4,4> const &tf,
	size_t nrows, size_t ncols, math::vec<3> const &scale, std::string const &texture_name)
{
    HeightfieldLoader(filename, this, renderer_ptr_).load_from_raw<boost::uint16_t>(tf, nrows, ncols, scale, texture_name);
}

Joint *Model::get_joint_by_id(std::string const &id)
{
	for (joints_type::iterator it = joints.begin(); it != joints.end(); it++) {
		if (it->id == id) return &*it;
	}

	throw std::runtime_error("joint with id = " + id + " not found in model");
}

Joint *Model::get_joint_by_name(std::string const &name)
{
	for (joints_type::iterator it = joints.begin(); it != joints.end(); it++) {
		if (it->name == name) return &*it;
	}

	throw std::runtime_error("joint with name = " + name + " not found in model");
}

Mesh *Model::get_mesh_by_id(std::string const &id)
{
	for (meshes_type::iterator it = meshes.begin(); it != meshes.end(); it++) {
		if (it->id == id) return &*it;
	}

	throw std::runtime_error("mesh with id = " + id + " not found");
}

Mesh *Model::get_mesh_by_name(std::string const &name)
{
	for (meshes_type::iterator it = meshes.begin(); it != meshes.end(); it++) {
		if (it->name == name) return &*it;
	}

	throw std::runtime_error("mesh with name = " + name + " not found");
}

Material *Model::get_material_by_name(std::string const &name)
{
	for (materials_type::iterator it = materials.begin(); it != materials.end(); it++) {
		if (it->get_name() == name) return &*it;
	}

	throw std::runtime_error("material with name = " + name + " not found");
}

void Model::set_animation_time(math::scalar t)
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


void Model::draw()
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
