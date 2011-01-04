#pragma once
//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2011 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <luabind/lua_include.hpp>
#include <math/vec.h>
#include "forward.h"
#include "texture.h"
#include "renderer.h"

namespace graphic
{

class Material: public boost::noncopyable {
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

	Material(RendererPtr const &renderer_ptr = RendererPtr());
	virtual ~Material();

	std::string const &get_name() const { return name_; }
	TexturePtr get_diffuse_texture(size_t index = 0) const { return diffuse_textures_.at(index); }
	TexturePtr get_normal_map_texture() const {	return normal_map_texture_; }

	void set_name(std::string const &name) { name_ = name; }
	void set_cull_mode(cull_mode_type cull_mode) { cull_mode_ = cull_mode; }
	void set_zwrite_enabled(bool zwrite_enabled) { zwrite_enabled_ = zwrite_enabled; }
	void set_alpha_blend_enabled(bool alpha_blend_enabled) { alpha_blend_enabled_ = alpha_blend_enabled; }
	void set_src_blend_mode(blend_mode_type src_blend_mode) { src_blend_mode_ = src_blend_mode; }
	void set_dest_blend_mode(blend_mode_type dest_blend_mode) { dest_blend_mode_ = dest_blend_mode; }

	void set_diffuse_texture(std::string const &filename, size_t index = 0);
	void set_normal_map_texture(std::string const &filename);

	virtual void select();

	static void bind(lua_State *L);

protected:
	std::string name_;

	RendererPtr renderer_;

	cull_mode_type cull_mode_;
	bool zwrite_enabled_;
	bool alpha_blend_enabled_;
	blend_mode_type src_blend_mode_, dest_blend_mode_;

	std::vector<TexturePtr> diffuse_textures_;
	TexturePtr normal_map_texture_;
};

}
