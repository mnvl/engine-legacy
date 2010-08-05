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

#include <boost/shared_ptr.hpp>
#include <luabind/lua_include.hpp>
#include <math/vec.h>
#include "texture.h"
#include "renderer.h"

namespace graphic
{

typedef boost::shared_ptr<class Material> MaterialPtr;

class Material {
public:
	Material(boost::shared_ptr<Renderer> const &renderer_ptr = boost::shared_ptr<Renderer>());
	~Material();

	void set_name(std::string const &name)
	{
		name_ = name;
	}

	std::string const &get_name() const
	{
		return name_;
	}

	boost::shared_ptr<Texture> get_diffuse_texture() const
	{
		return diffuse_texture_;
	}

	boost::shared_ptr<Texture> get_normal_map_texture() const
	{
		return normal_map_texture_;
	}

	void set_diffuse_texture(std::string const &filename);
	void set_normal_map_texture(std::string const &filename);

	void select();

	static void bind(lua_State *L);

private:
	std::string name_;

	boost::shared_ptr<Renderer> renderer_;
	boost::shared_ptr<Texture> diffuse_texture_;
	boost::shared_ptr<Texture> normal_map_texture_;
};

}
