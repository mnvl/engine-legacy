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
#include <boost/enable_shared_from_this.hpp>
#include <luabind/lua_include.hpp>
#include <math/matrix.h>
#include <platform/window.h>
#include "forward.h"
#include "camera.h"
#include "vertex_traits.h"
#include "vertex_array.h"
#include "index_array.h"

namespace graphic
{

class Renderer: public boost::enable_shared_from_this<Renderer>, public boost::noncopyable {
public:
	Renderer(boost::shared_ptr<platform::Window> const &window);
	virtual ~Renderer();

	virtual void postconstruct() = 0;

	virtual int get_screen_width() const = 0;
	virtual int get_screen_height() const = 0;

	virtual void begin_frame() = 0;
	virtual void finish_frame() = 0;

	virtual VertexBufferPtr create_vertex_buffer(VertexTraits const *vertex_traits,
		size_t size, bool dynamic = false, bool writeonly = false) = 0;
	virtual IndexBufferPtr create_index_buffer(unsigned index_format, size_t size,
		bool dynamic = false, bool writeonly = false) = 0;

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

	virtual TexturePtr create_texture(std::string const &filename) = 0;
	virtual FontPtr create_font(std::string const &face_name, int height = 16) = 0;
	virtual ShaderPtr create_shader() = 0;

	virtual UberShaderPtr get_uber_shader() const = 0;

	virtual DrawCallPtr create_draw_call() = 0;
	virtual MeshDrawCallPtr create_mesh_draw_call() = 0;

    static void bind(lua_State *L);

protected:
	boost::shared_ptr<platform::Window> const window_;
};

}
