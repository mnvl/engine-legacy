#pragma once

#include <boost/shared_ptr.hpp>
#include <math/vec.h>
#include <graphic/texture.h>
#include <graphic/renderer.h>

namespace graphic {

class Material {
public:
	Material(Renderer *renderer_ptr = 0);
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

	Renderer *renderer_ptr_;
	D3DMATERIAL9 properties_;

	boost::shared_ptr<Texture> texture_ptr_;
};

}
