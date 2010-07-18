//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <boost/unordered_map.hpp>
#include "collada_loader.h"
#include "heightfield_loader.h"
#include "model.h"

namespace graphic {

Model::Model(boost::shared_ptr<Renderer> const &renderer_ptr, boost::shared_ptr<Scheduler> const &scheduler_ptr):
	renderer_(renderer_ptr),
	scheduler_(scheduler_ptr)
{
}

Model::Model(Model const &model):
	materials(model.materials),
	renderer_(model.renderer_),
	scheduler_(model.scheduler_)
{
	boost::unordered_map<JointPtr, JointPtr> joints_tt;
	boost::unordered_map<MeshPtr, MeshPtr> meshes_tt;

	joints.reserve(model.joints.size());
	animations.reserve(model.animations.size());
	meshes.reserve(model.meshes.size());

	for (size_t i = 0; i < model.joints.size(); i++)
	{
		JointPtr source = model.joints[i];
		JointPtr joint(new Joint(*source));

		joints.push_back(joint);
		joints_tt.insert(std::make_pair(source, joint));
	}

	for (animations_type::const_iterator it = model.animations.begin(); it != model.animations.end(); it++)
	{
		animations.push_back(AnimationPtr(new Animation(**it)));
	}

	for (meshes_type::const_iterator it = model.meshes.begin(); it != model.meshes.end(); it++)
	{
		MeshPtr source = *it;

		MeshPtr mesh = source->shallow_copy();
		mesh->set_model(this);

		meshes.push_back(mesh);

		meshes_tt.insert(std::make_pair(source, mesh));
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

		if (MeshPtr mesh_ptr = (*it)->mesh_ptr.lock())
		{
			(*it)->mesh_ptr = meshes_tt.find(mesh_ptr)->second;
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
    HeightfieldLoader(filename, this, renderer_).load_from_raw<boost::int16_t>(tf, nrows, ncols, scale, texture_name);
}

void Model::load_from_raw_u16_heightfield(std::string const &filename, math::matrix<4,4> const &tf,
	size_t nrows, size_t ncols, math::vec<3> const &scale, std::string const &texture_name)
{
    HeightfieldLoader(filename, this, renderer_).load_from_raw<boost::uint16_t>(tf, nrows, ncols, scale, texture_name);
}

void Model::check()
{
	for (meshes_type::const_iterator it = meshes.begin(); it != meshes.end(); ++it)
	{
		(*it)->check();
	}
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
