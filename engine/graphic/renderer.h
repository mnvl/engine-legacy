#pragma once

#include <d3d9.h>
#include <boost/intrusive_ptr.hpp>
#include <engine/util/boost_com_intrusive_ptr.h>
#include <engine/system/window.h>

namespace engine {
namespace graphic {

class renderer {
public:
	renderer(system::window *window_ptr, int width = 800, int height = 600, bool fullscreen = false);
	void tick();

private:
	system::window *window_ptr_;
	int width_, height_;
	bool fullscreen_;

	boost::intrusive_ptr<IDirect3D9> direct3d_ptr_;
	boost::intrusive_ptr<IDirect3DDevice9> device_ptr_;
};

}
}