#pragma once
//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2011 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <vector>
#include <boost/noncopyable.hpp>
#include <luabind/lua_include.hpp>
#include <math/heightfield.h>
#include <graphic/forward.h>
#include <graphic/vertex_array.h>
#include <graphic/index_array.h>

namespace pirates
{

class Heightfield: public boost::noncopyable {
public:
	static unsigned const BINARY_FORMAT_VERSION = 1;

	static size_t const TEXTURE_SEABED = 0;
	static size_t const TEXTURE_SAND  = 1;
	static size_t const TEXTURE_GRASS = 2;

	Heightfield(graphic::RendererPtr const &renderer, graphic::SchedulerPtr const &scheduler,
		graphic::CameraPtr const &camera, math::matrix<4,4> const &tf,
		size_t nrows, size_t ncols, math::vec<3> const &scale);
	~Heightfield();

	void set_texture(std::string const &filename, size_t type);

	void update();
	void draw();

	math::heightfield<> const &get_storage() const { return heightfield_; }

	void add(math::vec<3> const &position, math::scalar delta);

	void load(std::string const &filename);
	void save(std::string const &filename);

	static void bind(lua_State *L);

private:
	graphic::RendererPtr const renderer_;
	graphic::SchedulerPtr const scheduler_;
	math::heightfield<> heightfield_;
	boost::shared_ptr<graphic::VertexArrayPNTB> vertex_array_;
	boost::shared_ptr<graphic::IndexArray16> index_array_;
	graphic::MaterialPtr material_;
	graphic::UberDrawCallPtr draw_call_;
};

}

