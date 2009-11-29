
#include <exception/directx_error.h>
#include <graphic/renderer.h>
#include <graphic/font.h>

namespace graphic {

Font::Font(boost::shared_ptr<Renderer> const &renderer, std::string const &face_name, int height) {
	HRESULT hr;

	ID3DXFont *font_ptr;
	hr = D3DXCreateFont(renderer->get_device(), height, 0, FW_MEDIUM, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, face_name.c_str(), &font_ptr);
	if (FAILED(hr)) throw exception::directx_error(hr);

	font_ptr_.reset(font_ptr);
	font_ptr->Release();
}

Font::~Font() {
}

}
