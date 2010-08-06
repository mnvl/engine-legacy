#pragma once
//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <d3d9.h>
#include <d3dx9.h>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/intrusive_ptr.hpp>
#include <luabind/lua_include.hpp>
#include <math/vec.h>
#include <util/boost_com_intrusive_ptr.h>
#include <boost/noncopyable.hpp>

namespace graphic
{

class Renderer;

class Font: public boost::noncopyable
{
	friend Renderer;

public:
	~Font();

	ID3DXFont *get() const { return font_.get(); }

	static DWORD const FLAG_ALIGN_LEFT = DT_LEFT;
	static DWORD const FLAG_ALIGN_HCENTER = DT_CENTER;
	static DWORD const FLAG_ALIGN_RIGHT = DT_RIGHT;
	static DWORD const FLAG_ALIGN_TOP = DT_TOP;
	static DWORD const FLAG_ALIGN_VCENTER = DT_VCENTER;
	static DWORD const FLAG_ALIGN_BOTTOM = DT_BOTTOM;

	void draw(math::vec<2> const &left_top, math::vec<2> const &right_bottom, char const *text, unsigned align,
		math::vec<3> const &color);

	virtual void handle_lost_device();
	virtual void handle_reset_device();

	static void bind(lua_State *L);

private:
	boost::intrusive_ptr<ID3DXFont> font_;

	Font(boost::shared_ptr<Renderer> const &renderer, std::string const &face_name, int height = 16);
};

}
