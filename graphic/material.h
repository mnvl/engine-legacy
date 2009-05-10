#pragma once

#include <graphic/renderer.h>
#include <util/boost_com_intrusive_ptr.h>

namespace engine {
namespace graphic {

class material {
public:
	material(renderer *renderer_ptr);
	~material();

	void set_texture(char const *filename);
	void select();

private:
	renderer *renderer_ptr_;
	D3DMATERIAL9 white_;
	boost::intrusive_ptr<IDirect3DTexture9> texture_ptr_;
};

}
}