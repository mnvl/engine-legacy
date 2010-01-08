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

#include <graphic/material.h>
#include <graphic/vertex_array.h>
#include <graphic/renderer.h>

namespace graphic
{
namespace ps
{

template<class Keeper>
class BillboardDrawer
{
public:
	typedef Keeper keeper_t;

	BillboardDrawer(boost::shared_ptr<Renderer> const &renderer, size_t max_nparticles):
		renderer_(renderer),
		max_nparticles_(max_nparticles),
		nparticles_(0)
	{
		vertex_array_ = renderer->create_vertex_array<graphic::vertex_pnt>(max_nparticles_ * VERTICES_PER_PARTICLE, true, true);
	}

	void set_material(boost::shared_ptr<Material> const &material)
	{
		material_ = material;
	}

	void preprocess(keeper_t const &keeper)
	{
		Camera const &camera = renderer_->get_camera();

		math::vec<3> normal = -camera.get_view_dir();

		vertex_pnt *v = vertex_array_->lock(true);
		nparticles_ = 0;

		for (keeper_t::iterator it = keeper.begin(); it != keeper.end(); ++it)
		{
			math::vec<3> up = camera.get_up_dir() * it->get_size();
			math::vec<3> cross = camera.get_cross_dir() * it->get_size();

			math::vec<3> position = it->get_position() + up / math::scalar(2) - cross / math::scalar(2);

			v->position = position;
			v->normal = normal;
			v->texcoords.set(0, 0);
			v++;

			v->position = position + cross;
			v->normal = normal;
			v->texcoords.set(0, 0);
			v++;

			v->position = position + cross - up;
			v->normal = normal;
			v->texcoords.set(0, 0);
			v++;

			v[0] = v[-1];
			v++;

			v->position = position - up;
			v->normal = normal;
			v->texcoords.set(0, 0);
			v++;

			v[0] = v[-5];
			v++;

			if (++nparticles_ > max_nparticles_)
			{
				throw std::logic_error("trying to preprocess more particles than allowed");
			}
		}

		vertex_array_->unlock();
	}

	void draw()
	{
		if (nparticles_)
		{
			material_->select();
			renderer_->draw_triangles(vertex_array_.get(), nparticles_);
		}
	}

private:
	static size_t const VERTICES_PER_PARTICLE = 6;

	boost::shared_ptr<Renderer> const renderer_;

	size_t const max_nparticles_;
	size_t nparticles_;

	boost::shared_ptr<Material> material_;
	boost::shared_ptr<VertexArrayPNT> vertex_array_;
};

}
}
