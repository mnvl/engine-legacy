#pragma once

#include <deque>
#include <d3d9.h>
#include <d3dx9.h>
#include <boost/intrusive_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <util/boost_com_intrusive_ptr.h>
#include <platform/window.h>

namespace graphic {

class camera;
class font;
class material;

class renderer {
public:
	renderer(platform::window *window_ptr, int width = 800, int height = 600, bool fullscreen = false);
	~renderer();

	void begin_frame();
	void finish_frame();

	IDirect3DDevice9 *get_device() const { return device_ptr_.get(); }
	font *get_default_font() const { return default_font_.get(); }
	material *get_default_material() const { return default_material_.get(); }

	void select(camera const &c);

private:
	platform::window *window_ptr_;
	int width_, height_;
	bool fullscreen_;

	boost::intrusive_ptr<IDirect3D9> direct3d_ptr_;
	boost::intrusive_ptr<IDirect3DDevice9> device_ptr_;

	boost::scoped_ptr<font> default_font_;
	boost::scoped_ptr<material> default_material_;

	double frame_start_time_, last_frame_time_;
	std::deque<double> frame_times_;
};

}
