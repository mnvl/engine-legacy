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
#include <math/vec.h>
#include <graphic/texture.h>
#include <graphic/renderer.h>

namespace graphic
{

typedef boost::shared_ptr<class Material> MaterialPtr;

class Material {
public:
	Material(boost::shared_ptr<Renderer> const &renderer_ptr = boost::shared_ptr<Renderer>());
	~Material();

	void set_diffuse(math::vec<4> const &v)
	{
		properties_.Specular.r = v.r;
		properties_.Specular.g = v.g;
		properties_.Specular.b = v.b;
		properties_.Specular.a = v.a;
	}

	void set_ambient(math::vec<4> const &v)
	{
		properties_.Ambient.r = v.r;
		properties_.Ambient.g = v.g;
		properties_.Ambient.b = v.b;
		properties_.Ambient.a = v.a;
	}

	void set_specular(math::vec<4> const &v)
	{
		properties_.Specular.r = v.r;
		properties_.Specular.g = v.g;
		properties_.Specular.b = v.b;
		properties_.Specular.a = v.a;
	}

	void set_emissive(math::vec<4> const &v)
	{
		properties_.Specular.r = v.r;
		properties_.Specular.g = v.g;
		properties_.Specular.b = v.b;
		properties_.Specular.a = v.a;
	}

	void set_power(math::scalar s)
	{
		properties_.Power = s;
	}

	void set_name(std::string const &name)
	{
		name_ = name;
	}

	std::string const &get_name() const
	{
		return name_;
	}

	std::string const &get_texture_name() const
	{
		return texture_ptr_->get_filename();
	}

	void set_texture(std::string const &filename);

	void select();

private:
	std::string name_;

	boost::shared_ptr<Renderer> renderer_ptr_;
	D3DMATERIAL9 properties_;

	boost::shared_ptr<Texture> texture_ptr_;
};

}
