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

	Model(boost::shared_ptr<Renderer> const &renderer_ptr = boost::shared_ptr<Renderer>());
	Model(Model const &model); // makes shallow copy of meshes and materials, and full copy of joints and animations
	virtual ~Model();

	boost::shared_ptr<Renderer> const &get_renderer() const
	{
		return renderer_ptr_;
	}

	JointPtr get_joint_by_id(std::string const &name);
	JointPtr get_joint_by_name(std::string const &name);
	MeshPtr get_mesh_by_id(std::string const &id);
	MeshPtr get_mesh_by_name(std::string const &name);
	MaterialPtr get_material_by_name(std::string const &name);

	virtual JointPtr add_joint();
	virtual MeshPtr add_mesh() = 0;

    void load_from_collada(std::string const &filename);
    void load_from_raw_i16_heightfield(std::string const &filename, math::matrix<4,4> const &tf,
		size_t nrows, size_t ncols, math::vec<3> const &scale, std::string const &texture_name);
    void load_from_raw_u16_heightfield(std::string const &filename, math::matrix<4,4> const &tf,
		size_t nrows, size_t ncols, math::vec<3> const &scale, std::string const &texture_name);

	// animations blending could be easily implemented adding another function that will clear
	// all Joint::matrix variables to null matrix (all elements == zero) and specifying animation
	// and blend factor in this function
    virtual void set_animation_time(math::scalar t);

    virtual void draw();

    static void bind(lua_State *L);

protected:
	boost::shared_ptr<Renderer> const renderer_ptr_;
};

}
