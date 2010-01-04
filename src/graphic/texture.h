#pragma once

#include <lua.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <boost/shared_ptr.hpp>
#include <boost/intrusive_ptr.hpp>
#include <util/boost_com_intrusive_ptr.h>
#include <exception/directx_error.h>
#include "resource.h"

namespace graphic
{

class Renderer;

class Texture: public Resource {
	friend Renderer;

public:
	Texture(boost::shared_ptr<Renderer> const &renderer_ptr, std::string const &filename);
	~Texture();

	std::string const &get_filename() const
	{
		return filename_;
	}

	IDirect3DTexture9 *get() const
	{
		return texture_ptr_.get();
	}

	void select();

	virtual void handle_device_lost();
	virtual void restore_after_device_lost();

	static void bind(lua_State *L);

private:
	boost::shared_ptr<Renderer> renderer_ptr_;
	std::string filename_;
	boost::intrusive_ptr<IDirect3DTexture9> texture_ptr_;

	void load_from_file(std::string const &filename);
};

}
