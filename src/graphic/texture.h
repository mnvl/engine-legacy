#pragma once

#include <lua.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <boost/shared_ptr.hpp>
#include <boost/intrusive_ptr.hpp>
#include <util/boost_com_intrusive_ptr.h>
#include <exception/directx_error.h>
#include <graphic/renderer.h>

namespace graphic
{

class Texture {
public:
	Texture(boost::shared_ptr<Renderer> const &renderer_ptr);
	~Texture();

	void load_from_file(std::string const &filename);

	std::string const &get_filename() const
	{
		return filename_;
	}

	IDirect3DTexture9 *get() const
	{
		return texture_ptr_.get();
	}

	static void bind(lua_State *L);

private:
	boost::shared_ptr<Renderer> renderer_ptr_;
	std::string filename_;
	boost::intrusive_ptr<IDirect3DTexture9> texture_ptr_;
};

}
