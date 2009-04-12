#pragma once

#include <d3d9.h>
#include <boost/intrusive_ptr.hpp>
#include <engine/system/window.h>

namespace boost {
	inline void intrusive_ptr_add_ref(IUnknown *unknown) { unknown->AddRef(); }
	inline void intrusive_ptr_release(IUnknown *unknown) { unknown->Release(); }
}

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