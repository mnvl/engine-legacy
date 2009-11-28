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
#include "resource_manager.h"
#include "vertex_array.h"

namespace graphic {

class Camera;
class Font;
class Material;

class Renderer: public boost::enable_shared_from_this<Renderer>, public boost::noncopyable {
public:
	Renderer(boost::shared_ptr<platform::Window> const &window, int width = 800, int height = 600,
		bool fullscreen = false);
	~Renderer();

	void postconstruct();

	void begin_frame();
	void finish_frame();

	IDirect3DDevice9 *get_device() const { return device_ptr_.get(); }
    ResourceManager *get_resource_manager() const { return resource_manager_.get(); }
	math::matrix<4,4> const &get_view_matrix() const { return view_matrix_; }
	math::matrix<4,4> const &get_projection_matrix() const { return projection_matrix_; }
    math::matrix<4,4> const &get_world_matrix() const { return world_matrix_; }

	void set_view_projection_from_camera(Camera const &c);
    void set_world_matrix(math::matrix<4,4> const &tf);

	void draw_triangles(VertexBuffer *buffer, size_t triangles_count);

    static void bind(lua_State *L);

private:
	boost::shared_ptr<platform::Window> const window_ptr_;
	int width_, height_;
	bool fullscreen_;

    math::matrix<4,4> world_matrix_;
	math::matrix<4,4> view_matrix_;
	math::matrix<4,4> projection_matrix_;

	boost::intrusive_ptr<IDirect3D9> direct3d_ptr_;
	boost::intrusive_ptr<IDirect3DDevice9> device_ptr_;

    boost::scoped_ptr<ResourceManager> resource_manager_;

	double frame_start_time_, last_frame_time_;
	std::deque<double> frame_times_;
};

}
