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
#include "resource.h"
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

    void set_world_matrix(math::matrix<4,4> const &tf);
	void set_camera(Camera const &camera); // stores copy of camera

	math::matrix<4,4> const &get_world_matrix() const { return world_matrix_; }
	Camera const &get_camera() const { return camera_; }
	math::matrix<4,4> const &get_view_matrix() const { return camera_.get_view_matrix(); }
	math::matrix<4,4> const &get_projection_matrix() const { return camera_.get_projection_matrix(); }

	template <class V> boost::shared_ptr<VertexArray<V> >
	create_vertex_array(size_t count, bool dynamic = false, bool writeonly = false)
	{
		boost::shared_ptr<VertexArray<V> > vertex_array(new VertexArray<V>(shared_from_this(), count, dynamic, writeonly));
		register_resource(boost::static_pointer_cast<Resource>(vertex_array));
		return vertex_array;
	}

	template <class I> boost::shared_ptr<IndexArray<I> >
	create_index_array(size_t count, bool dynamic = false, bool writeonly = false)
	{
		boost::shared_ptr<IndexArray<I> > index_array(new IndexArray<I>(shared_from_this(), count, dynamic, writeonly));
		register_resource(boost::static_pointer_cast<Resource>(index_array));
		return index_array;
	}

	boost::shared_ptr<Texture> create_texture(std::string const &filename);
	boost::shared_ptr<Font> create_font(std::string const &face_name, int height = 16);

	void register_resource(boost::shared_ptr<Resource> const &resource);

	void draw_triangles(VertexBuffer *buffer, size_t triangles_count);
	void draw_triangles(VertexBuffer *buffer, size_t minimal_vertex_index, size_t triangles_count);

	void disable_all_tests();

    static void bind(lua_State *L);

private:
	boost::shared_ptr<platform::Window> const window_;
	int screen_width_, screen_height_;
	bool fullscreen_;
	D3DPRESENT_PARAMETERS pp_;

	Camera camera_;
    math::matrix<4,4> world_matrix_;

	boost::intrusive_ptr<IDirect3D9> direct3d_;
	boost::intrusive_ptr<IDirect3DDevice9> device_;

	boost::signals2::signal0<void> signal_lost_device_;
	boost::signals2::signal0<void> signal_reset_device_;

    boost::scoped_ptr<ShadersLibrary> shaders_library_;
	boost::scoped_ptr<VertexDeclarations> vertex_declarations_;

	void handle_resize();
};

}
