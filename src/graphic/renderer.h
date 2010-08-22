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

#include <d3d9.h>
#include <d3dx9.h>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/intrusive_ptr.hpp>
#include <luabind/lua_include.hpp>
#include <math/matrix.h>
#include <util/boost_com_intrusive_ptr.h>
#include <platform/window.h>
#include "camera.h"
#include <boost/noncopyable.hpp>
#include "vertex_traits.h"
#include "vertex_array.h"
#include "index_array.h"

namespace graphic
{

class ShadersLibrary;
class VertexDeclarations;
class Texture;
class Material;
class Font;
class Shader;

class Renderer: public boost::enable_shared_from_this<Renderer>, public boost::noncopyable {
public:
	Renderer(boost::shared_ptr<platform::Window> const &window, int width = 0, int height = 0,
		bool fullscreen = false);
	~Renderer();

	int get_screen_width() const { return screen_width_; }
	int get_screen_height() const { return screen_height_; }

	void postconstruct();
	void reset_state();
	void reset_device();

	void begin_frame();
	void finish_frame();

	IDirect3DDevice9 *get_device() const { return device_.get(); }
	VertexDeclarations *get_vertex_declarations() const { return vertex_declarations_.get(); }
	ShadersLibrary *get_shaders_library() const { return shaders_library_.get(); }

	VertexBufferPtr create_vertex_buffer(VertexTraits const *vertex_traits, size_t size, bool dynamic = false, bool writeonly = false);
	IndexBufferPtr create_index_buffer(unsigned index_format, size_t size, bool dynamic = false, bool writeonly = false);

	template <class V> boost::shared_ptr< VertexArray<V> >
	create_vertex_array(size_t count, bool dynamic = false, bool writeonly = false)
	{
		return boost::shared_ptr< VertexArray<V> >(
			new VertexArray<V>(
				create_vertex_buffer(VertexTraits::get<V>(), sizeof(V) * count, dynamic, writeonly)));
	}

	template <class I> boost::shared_ptr<IndexArray<I> >
	create_index_array(size_t count, bool dynamic = false, bool writeonly = false)
	{
		return boost::shared_ptr< IndexArray<I> >(
			new IndexArray<I>(
				create_index_buffer(INDEX_FORMAT_U16, sizeof(I) * count, dynamic, writeonly)));
	}

	TexturePtr create_texture(std::string const &filename);
	boost::shared_ptr<Font> create_font(std::string const &face_name, int height = 16);

	template<class T>
	void register_resource(boost::shared_ptr<T> const &resource)
	{
		signal_lost_device_.connect
			(boost::signals2::signal0<void>::slot_type(&T::handle_lost_device, resource.get()).track(resource));

		signal_reset_device_.connect
			(boost::signals2::signal0<void>::slot_type(&T::handle_reset_device, resource.get()).track(resource));
	}

    static void bind(lua_State *L);

private:
	boost::shared_ptr<platform::Window> const window_;
	int screen_width_, screen_height_;
	bool fullscreen_;
	D3DPRESENT_PARAMETERS pp_;

	boost::intrusive_ptr<IDirect3D9> direct3d_;
	boost::intrusive_ptr<IDirect3DDevice9> device_;

	boost::signals2::signal0<void> signal_lost_device_;
	boost::signals2::signal0<void> signal_reset_device_;

    boost::scoped_ptr<ShadersLibrary> shaders_library_;
	boost::scoped_ptr<VertexDeclarations> vertex_declarations_;

	void handle_resize();
};

}
