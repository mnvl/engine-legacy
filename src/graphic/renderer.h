#pragma once

#include <deque>
#include <d3d9.h>
#include <d3dx9.h>
#include <boost/scoped_ptr.hpp>
#include <boost/intrusive_ptr.hpp>
#include <lua.h>
#include <math/matrix.h>
#include <util/boost_com_intrusive_ptr.h>
#include <platform/window.h>
#include <graphic/resource_manager.h>

namespace graphic {

class Camera;
class Font;
class Material;

class Renderer {
public:
	Renderer(platform::Window *window_ptr, int width = 800, int height = 600, bool fullscreen = false);
	~Renderer();

	void begin_frame();
	void finish_frame();

	IDirect3DDevice9 *get_device() const { return device_ptr_.get(); }
    ResourceManager *get_resource_manager() const { return resource_manager_.get(); }
	math::matrix<4,4> const &get_view_matrix() const { return view_matrix_; }
	math::matrix<4,4> const &get_projection_matrix() const { return projection_matrix_; }

	void set_view_projection_from_camera(Camera const &c);

    static void bind(lua_State *L);

private:
	platform::Window *window_ptr_;
	int width_, height_;
	bool fullscreen_;

	math::matrix<4,4> view_matrix_;
	math::matrix<4,4> projection_matrix_;

	boost::intrusive_ptr<IDirect3D9> direct3d_ptr_;
	boost::intrusive_ptr<IDirect3DDevice9> device_ptr_;

    boost::scoped_ptr<ResourceManager> resource_manager_;

	double frame_start_time_, last_frame_time_;
	std::deque<double> frame_times_;
};

}
