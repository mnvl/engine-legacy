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
#include <math/heightfield.h>
#include <graphic/forward.h>
#include <graphic/vertex_array.h>
#include <graphic/index_array.h>

namespace pirates
{

class Heightfield: public boost::noncopyable {
public:
	static size_t const TEXTURE_WATER = 0;
	static size_t const TEXTURE_SAND  = 1;
	static size_t const TEXTURE_GRASS = 2;

	Heightfield(graphic::RendererPtr const &renderer, math::matrix<4,4> const &tf,
		size_t nrows, size_t ncols, math::vec<3> const &scale);
	~Heightfield();

	void set_texture(std::string const &filename, size_t type);

	void update();
	void draw();

private:
	graphic::RendererPtr const renderer_;
	math::heightfield<> heightfield_;
	boost::shared_ptr<graphic::VertexArrayPNTB> vertex_array_;
	boost::shared_ptr<graphic::IndexArray16> index_array_;
	graphic::MaterialPtr material_;
	graphic::UberDrawCallPtr draw_call_;
};

}

