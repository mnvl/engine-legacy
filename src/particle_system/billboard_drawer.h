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

#include <luabind/lua_include.hpp>
#include <math/aabb.h>
#include <graphic/material.h>
#include <graphic/vertex_array.h>
#include <graphic/draw_call.h>
#include <graphic/scheduler.h>
#include <graphic/renderer.h>

namespace particle_system
{

class BillboardDrawer
{
public:
	BillboardDrawer(graphic::RendererPtr const &renderer,
		boost::shared_ptr<graphic::Scheduler> const &scheduler,
		boost::shared_ptr<graphic::Camera> const &camera,
		size_t max_nparticles)
	:
		renderer_(renderer),
		scheduler_(scheduler),
		camera_(camera),
		max_nparticles_(max_nparticles),
		nparticles_(0)
	{
		vertex_array_ = renderer->create_vertex_array<graphic::VERTEX_PNT>(max_nparticles_ * VERTICES_PER_PARTICLE, true, true);

		draw_call_.reset(new graphic::DrawCall(renderer_));
		draw_call_->set_camera(camera_);
		draw_call_->set_vertex_buffer(vertex_array_->get_vertex_buffer());
		draw_call_->set_priority(1000);
	}

	void set_material(boost::shared_ptr<graphic::Material> const &material)
	{
		material->set_cull_mode(graphic::Material::CULL_NONE);
		material->set_zwrite_enabled(false);
		material->set_alpha_blend_enabled(true);
		material->set_dest_blend_mode(graphic::Material::BLEND_ONE);
		draw_call_->set_material(material);
	}

	template<class Keeper>
	void preprocess(Keeper const &keeper)
	{
		math::aabb<3> bounds;
		bounds.null();

		math::vec<3> normal = -camera_->get_view_dir();

		graphic::VERTEX_PNT *v = vertex_array_->lock(true);
		nparticles_ = 0;

		for (Keeper::iterator it = keeper.begin(); it != keeper.end(); ++it)
		{
			math::vec<3> up = camera_->get_up_dir() * it->get_size();
			math::vec<3> cross = camera_->get_cross_dir() * it->get_size();

			math::vec<3> position = it->get_position() + up / math::scalar(2) - cross / math::scalar(2);

			v->position = position;
			v->normal = normal;
			v->texcoords.set(0, 0);
			bounds.extend(v->position);
			v++;

			v->position = position + cross - up;
			v->normal = normal;
			v->texcoords.set(1, 1);
			bounds.extend(v->position);
			v++;

			v->position = position + cross;
			v->normal = normal;
			v->texcoords.set(0, 1);
			bounds.extend(v->position);
			v++;

			v[0] = v[-3];
			v++;

			v->position = position - up;
			v->normal = normal;
			v->texcoords.set(1, 0);
			bounds.extend(v->position);
			v++;

			v[0] = v[-4];
			v++;

			if (++nparticles_ > max_nparticles_)
			{
				throw std::logic_error("trying to preprocess more particles than allowed");
			}
		}

		vertex_array_->unlock();

		draw_call_->set_vertices_count(nparticles_ * VERTICES_PER_PARTICLE);
		draw_call_->set_triangles_count(nparticles_ * TRIANGLES_PER_PARTICLE);
		draw_call_->set_obb(math::obb<3>(bounds));
		draw_call_->commit();
	}

	void draw()
	{
		if (!nparticles_) return;
		scheduler_->add(draw_call_);
	}

	static void bind(lua_State *L);

private:
	static size_t const TRIANGLES_PER_PARTICLE = 2;
	static size_t const VERTICES_PER_PARTICLE = 6;

	graphic::RendererPtr const renderer_;
	boost::shared_ptr<graphic::Scheduler> const scheduler_;
	boost::shared_ptr<graphic::Camera> const camera_;

	size_t const max_nparticles_;
	size_t nparticles_;

	boost::shared_ptr<graphic::VertexArrayPNT> vertex_array_;
	boost::shared_ptr<graphic::DrawCall> draw_call_;
};

}
