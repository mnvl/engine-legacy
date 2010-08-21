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
	enum cull_mode_type
	{
		CULL_CW,
		CULL_CCW,
		CULL_NONE
	};

	enum blend_mode_type
	{
		BLEND_ZERO,
		BLEND_ONE
	};

	Material(boost::shared_ptr<Renderer> const &renderer_ptr = boost::shared_ptr<Renderer>());
	~Material();

	std::string const &get_name() const { return name_; }
	boost::shared_ptr<Texture> get_diffuse_texture() const { return diffuse_texture_; }
	boost::shared_ptr<Texture> get_normal_map_texture() const {	return normal_map_texture_; }

	void set_name(std::string const &name) { name_ = name; }
	void set_cull_mode(cull_mode_type cull_mode) { cull_mode_ = cull_mode; }
	void set_zwrite_enabled(bool zwrite_enabled) { zwrite_enabled_ = zwrite_enabled; }
	void set_alpha_blend_enabled(bool alpha_blend_enabled) { alpha_blend_enabled_ = alpha_blend_enabled; }
	void set_src_blend_mode(blend_mode_type src_blend_mode) { src_blend_mode_ = src_blend_mode; }
	void set_dest_blend_mode(blend_mode_type dest_blend_mode) { dest_blend_mode_ = dest_blend_mode; }

	void set_diffuse_texture(std::string const &filename);
	void set_normal_map_texture(std::string const &filename);

	void select();

	static void bind(lua_State *L);

private:
	std::string name_;

	boost::shared_ptr<Renderer> renderer_;

	cull_mode_type cull_mode_;
	bool zwrite_enabled_;
	bool alpha_blend_enabled_;
	blend_mode_type src_blend_mode_, dest_blend_mode_;

	boost::shared_ptr<Texture> diffuse_texture_;
	boost::shared_ptr<Texture> normal_map_texture_;
};

}
