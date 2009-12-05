
#include <boost/unordered_map.hpp>
#include "collada_loader.h"
#include "heightfield_loader.h"
#include "model.h"

namespace graphic {

Model::Model(boost::shared_ptr<Renderer> const &renderer_ptr):
	renderer_ptr_(renderer_ptr)
{
}

Model::Model(Model const &model):
	meshes(model.meshes),
	materials(model.materials)
{
	boost::unordered_map<JointPtr, JointPtr> joints_tt;

	joints.reserve(model.joints.size());
	animations.reserve(model.animations.size());

	for (size_t i = 0; i < model.joints.size(); i++)
	{
		joints.push_back(JointPtr(new Joint(*model.joints[i])));
		joints_tt[model.joints[i]] = joints.back();
	}

	for (size_t i = 0; i < model.animations.size(); i++)
	{
		animations.push_back(AnimationPtr(new Animation(*model.animations[i])));
	}

	for (joints_type::iterator it = joints.begin(); it != joints.end(); it++)
	{
		if (JointPtr parent = (*it)->parent.lock())
		{
			(*it)->parent = joints_tt.find(parent)->second;
		}

		for (Joint::childs_type::iterator jt = (*it)->childs.begin(); jt != (*it)->childs.end(); jt++)
		{
			*jt = joints_tt.find(jt->lock())->second;
		}
	}

	for (animations_type::iterator it = animations.begin(); it != animations.end(); it++)
	{
		for (Animation::samplers_type::iterator jt = (*it)->samplers.begin(); jt != (*it)->samplers.end(); jt++)
		{
			if (JointPtr joint = jt->get_joint())
			{
				jt->set_joint(joints_tt.find(joint)->second);
			}
		}
	}
}

Model::~Model()
{
}

JointPtr Model::add_joint()
{
	joints.push_back(JointPtr(new Joint(joints.size())));
	return joints.back();
}

void Model::load_from_collada(std::string const &filename)
{
    ColladaLoader loader(this);
	loader.load(filename);
	loader.build_model();
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

JointPtr Model::get_joint_by_id(std::string const &id)
{
	for (joints_type::iterator it = joints.begin(); it != joints.end(); it++) {
		if ((*it)->id == id) return *it;
	}

	throw std::runtime_error("joint with id = " + id + " not found in model");
}

JointPtr Model::get_joint_by_name(std::string const &name)
{
	for (joints_type::iterator it = joints.begin(); it != joints.end(); it++) {
		if ((*it)->name == name) return *it;
	}

	throw std::runtime_error("joint with name = " + name + " not found");
}

MeshPtr Model::get_mesh_by_id(std::string const &id)
{
	for (meshes_type::iterator it = meshes.begin(); it != meshes.end(); it++) {
		if ((*it)->id == id) return *it;
	}

	throw std::runtime_error("mesh with id = " + id + " not found");
}

MeshPtr Model::get_mesh_by_name(std::string const &name)
{
	for (meshes_type::iterator it = meshes.begin(); it != meshes.end(); it++) {
		if ((*it)->name == name) return *it;
	}

	throw std::runtime_error("mesh with name = " + name + " not found");
}

MaterialPtr Model::get_material_by_name(std::string const &name)
{
	for (materials_type::iterator it = materials.begin(); it != materials.end(); it++) {
		if ((*it)->get_name() == name) return *it;
	}

	throw std::runtime_error("material with name = " + name + " not found");
}

void Model::set_animation_time(math::scalar t)
{
	for (animations_type::iterator it = animations.begin(); it != animations.end(); it++)
	{
		(*it)->set_time(t);
	}

	for (joints_type::iterator it = joints.begin(); it != joints.end(); it++) {
		if (!(*it)->parent.lock()) {
			(*it)->build_frame_transforms();
		}
	}
}

void Model::draw()
{
	for (joints_type::iterator it = joints.begin(); it != joints.end(); it++)
	{
		if (MeshPtr mesh_ptr = (*it)->mesh_ptr.lock())
		{
			mesh_ptr->draw(it->get());
		}
	}
}

}
