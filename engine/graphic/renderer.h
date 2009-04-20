#pragma once

#include <deque>
#include <d3d9.h>
#include <d3dx9.h>
#include <boost/intrusive_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <engine/util/boost_com_intrusive_ptr.h>
#include <engine/system/window.h>

namespace engine {
namespace graphic {

class camera;
class font;

class renderer {
public:
	renderer(system::window *window_ptr, int width = 800, int height = 600, bool fullscreen = false);
	~renderer();

	void begin_frame();
	void finish_frame();

	IDirect3DDevice9 *get_device() const { return device_ptr_.get(); }

	void select(camera const &c);

private:
	system::window *window_ptr_;
	int width_, height_;
	bool fullscreen_;

	boost::intrusive_ptr<IDirect3D9> direct3d_ptr_;
	boost::intrusive_ptr<IDirect3DDevice9> device_ptr_;

	boost::scoped_ptr<font> default_font_;

	double frame_start_time_, last_frame_time_;
	std::deque<double> frame_times_;
};

}
}