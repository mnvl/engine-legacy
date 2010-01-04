#pragma once

#include <deque>
#include <d3d9.h>
#include <d3dx9.h>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/intrusive_ptr.hpp>
#include <lua.h>
#include <math/matrix.h>
#include <util/boost_com_intrusive_ptr.h>
#include <platform/window.h>
#include "resource.h"
#include "shared_resources.h"
#include "vertex_array.h"
#include "texture.h"

namespace graphic
{

class Camera;
class Font;
class Material;
class Texture;
class IndexBuffer;

class Renderer: public boost::enable_shared_from_this<Renderer>, public boost::noncopyable {
public:
	Renderer(boost::shared_ptr<platform::Window> const &window, int width = 0, int height = 0,
		bool fullscreen = false);
	~Renderer();

	int const get_screen_width() const { return screen_width_; }
	int const get_screen_height() const { return screen_height_; }

	void postconstruct();

	void begin_frame();
	void finish_frame();

	IDirect3DDevice9 *get_device() const { return device_ptr_.get(); }
    SharedResources *get_shared_resources() const { return shared_resources_.get(); }
	math::matrix<4,4> const &get_view_matrix() const { return view_matrix_; }
	math::matrix<4,4> const &get_projection_matrix() const { return projection_matrix_; }
    math::matrix<4,4> const &get_world_matrix() const { return world_matrix_; }

	void set_view_projection_from_camera(Camera const &c);
    void set_world_matrix(math::matrix<4,4> const &tf);

	boost::shared_ptr<Texture> create_texture(std::string const &filename)
	{
		boost::shared_ptr<Texture> texture(new Texture(shared_from_this(), filename));
		resources_directory_->add_resource(texture);
		return texture;
	}

	boost::shared_ptr<VertexBuffer>
	create_vertex_buffer(size_t size, bool dynamic = false, bool writeonly = false)
	{
		boost::shared_ptr<VertexBuffer> vertex_buffer(new VertexBuffer(shared_from_this(), size, dynamic, writeonly));
		resources_directory_->add_resource(vertex_buffer);
		return vertex_buffer;
	}

	template <class V> boost::shared_ptr<VertexArray<V> >
	create_vertex_array(size_t count, bool dynamic = false, bool writeonly = false)
	{
		boost::shared_ptr<VertexArray<V> > vertex_array(new VertexArray<V>(shared_from_this(), count, dynamic, writeonly));
		resources_directory_->add_resource(vertex_array);
		return vertex_array;
	}

	void draw_triangles(VertexBuffer *buffer, size_t triangles_count);
	void draw_triangles(VertexBuffer *buffer, size_t first_vertex, size_t triangles_count);

	void disable_all_tests();

    static void bind(lua_State *L);

private:
	boost::shared_ptr<platform::Window> const window_ptr_;
	int screen_width_, screen_height_;
	bool fullscreen_;
	D3DPRESENT_PARAMETERS pp_;

    math::matrix<4,4> world_matrix_;
	math::matrix<4,4> view_matrix_;
	math::matrix<4,4> projection_matrix_;

	boost::intrusive_ptr<IDirect3D9> direct3d_ptr_;
	boost::intrusive_ptr<IDirect3DDevice9> device_ptr_;

    boost::scoped_ptr<SharedResources> shared_resources_;
	boost::scoped_ptr<ResourcesDirectory> resources_directory_;

	void handle_resize();
	static void thunk_resize(boost::weak_ptr<Renderer> renderer);
};

}
