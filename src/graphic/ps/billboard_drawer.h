//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------
#pragma once

#include <graphic/texture.h>
#include <graphic/vertex_array.h>
#include <graphic/renderer.h>

namespace graphic
{
namespace ps
{

template<class P>
class BillboardDrawer
{
public:
	typedef P particle_t;

	BillboardDrawer(boost::shared_ptr<Renderer> const &renderer, size_t max_nparticles):
		renderer_(renderer),
		max_nparticles_(max_nparticles),
		cursor_(0)
	{
		vertex_array_ = renderer->create_vertex_array<graphic::vertex_pnt>(max_nparticles_ * VERTICES_PER_PARTICLE, true, true);
	}

	void set_texture(std::string const &filename)
	{
		texture_ = renderer_->create_texture(filename);
	}

	void begin()
	{
		texture_->select();

		cursor_ = vertex_array_->lock(true);
		particles_to_draw_ = 0;
	}

	void draw(particle_t const *particle)
	{
		
	}

	void end()
	{
		vertex_array_->unlock();
		cursor_ = 0;
	}

private:
	static size_t const VERTICES_PER_PARTICLE = 6;

	boost::shared_ptr<Renderer> const renderer_;
	size_t const max_nparticles_;

	boost::shared_ptr<Texture> texture_;
	boost::shared_ptr<VertexArrayPNT> vertex_array_;

	vertex_pnt *cursor_;
	size_t particles_to_draw_;
};

}
}
