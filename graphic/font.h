#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <boost/intrusive_ptr.hpp>
#include <util/boost_com_intrusive_ptr.h>

namespace engine {
namespace graphic {

class renderer;

class font {
public:
	font(renderer *renderer, std::string const &face_name, int height = 16);
	~font();

	ID3DXFont *get_font() const { return font_ptr_.get(); }

private:
	boost::intrusive_ptr<ID3DXFont> font_ptr_;
};

}
}