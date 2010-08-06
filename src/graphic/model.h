#pragma once
//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <vector>
#include <boost/shared_ptr.hpp>
#include <luabind/lua_include.hpp>
#include "joint.h"
#include "animation.h"
#include "mesh.h"
#include "material.h"

namespace graphic
{

class Camera;
class Renderer;
class Scheduler;

class Model {
public:
	typedef std::vector<JointPtr> joints_type;
	typedef std::vector<AnimationPtr> animations_type;
	typedef std::vector<MeshPtr> meshes_type;
	typedef std::vector<MaterialPtr> materials_type;

	joints_type joints;
	meshes_type meshes;
	animations_type animations;
	materials_type materials;

	Model(boost::shared_ptr<Renderer> const &renderer_ptr, boost::shared_ptr<Scheduler> const &scheduler_ptr);
	Model(Model const &model); // makes shallow copy of meshes and materials, and full copy of joints and animations
	virtual ~Model();

	boost::shared_ptr<Renderer> const &get_renderer() const { return renderer_; }
	boost::shared_ptr<Scheduler> const &get_scheduler() const { return scheduler_; }

	JointPtr get_joint_by_id(std::string const &name);
	JointPtr get_joint_by_name(std::string const &name);
	MeshPtr get_mesh_by_id(std::string const &id);
	MeshPtr get_mesh_by_name(std::string const &name);
	MaterialPtr get_material_by_name(std::string const &name);

	virtual JointPtr add_joint();
	virtual MeshPtr add_mesh() = 0;

    void load_from_collada(std::string const &filename);

	// checks that model have been loaded correctly
	virtual void check();

	// animations blending could be easily implemented adding another function that will clear
	// all Joint::matrix variables to null matrix (all elements == zero) and specifying animation
	// and blend factor in this function
    virtual void set_animation_time(math::scalar t);

    virtual void draw(boost::shared_ptr<Camera> const &camera, math::matrix<4,4> const &world_matrix);

    static void bind(lua_State *L);

protected:
	boost::shared_ptr<Renderer> const renderer_;
	boost::shared_ptr<Scheduler> const scheduler_;
};

}
