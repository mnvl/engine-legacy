#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/intrusive_ptr.hpp>
#include <lua.h>
#include <math/vec.h>
#include <util/boost_com_intrusive_ptr.h>

namespace graphic
{

class Renderer;

class Font {
public:
	Font(boost::shared_ptr<Renderer> const &renderer, std::string const &face_name, int height = 16);
	~Font();

	ID3DXFont *get() const { return font_ptr_.get(); }

	static DWORD const FLAG_ALIGN_LEFT = DT_LEFT;
	static DWORD const FLAG_ALIGN_HCENTER = DT_CENTER;
	static DWORD const FLAG_ALIGN_RIGHT = DT_RIGHT;
	static DWORD const FLAG_ALIGN_TOP = DT_TOP;
	static DWORD const FLAG_ALIGN_VCENTER = DT_VCENTER;
	static DWORD const FLAG_ALIGN_BOTTOM = DT_BOTTOM;

	void draw(math::vec<2> const &left_top, math::vec<2> const &right_bottom, char const *text, unsigned align,
		math::vec<3> const &color);

	static void bind(lua_State *L);

private:
	boost::intrusive_ptr<ID3DXFont> font_ptr_;
};

}
