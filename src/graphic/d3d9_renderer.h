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
#include <boost/intrusive_ptr.hpp>
#include <util/boost_com_intrusive_ptr.h>
#include "renderer.h"

namespace graphic
{

class VertexDeclarations;
class ShadersLibrary;

class D3D9Renderer: public Renderer {
public:
	D3D9Renderer(boost::shared_ptr<platform::Window> const &window, int width = 0, int height = 0,
		bool fullscreen = false);
	virtual ~D3D9Renderer();

	virtual int get_screen_width() const;
	virtual int get_screen_height() const;

	virtual void postconstruct();

	void reset_state();
	void reset_device();

	virtual void begin_frame();
	virtual void finish_frame();

	IDirect3DDevice9 *get_device() const { return device_.get(); }
	VertexDeclarations *get_vertex_declarations() const { return vertex_declarations_.get(); }
	ShadersLibrary *get_shaders_library() const { return shaders_library_.get(); }

	virtual VertexBufferPtr create_vertex_buffer(VertexTraits const *vertex_traits,
		size_t size, bool dynamic = false, bool writeonly = false);
	virtual IndexBufferPtr create_index_buffer(unsigned index_format, size_t size,
		bool dynamic = false, bool writeonly = false);

	virtual TexturePtr create_texture(std::string const &filename);
	virtual boost::shared_ptr<Font> create_font(std::string const &face_name, int height = 16);

	template<class T>
	void register_resource(boost::shared_ptr<T> const &resource)
	{
		signal_lost_device_.connect
			(boost::signals2::signal0<void>::slot_type(&T::handle_lost_device, resource.get()).track(resource));

		signal_reset_device_.connect
			(boost::signals2::signal0<void>::slot_type(&T::handle_reset_device, resource.get()).track(resource));
	}

private:
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

