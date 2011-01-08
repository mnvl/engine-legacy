//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2011 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <luabind/luabind.hpp>
#include <graphic/uber_draw_call.h>
#include <graphic/material.h>
#include <graphic/scheduler.h>
#include <graphic/renderer.h>
#include "heightfield.h"

namespace pirates
{

Heightfield::Heightfield(graphic::RendererPtr const &renderer, graphic::SchedulerPtr const &scheduler,
	graphic::CameraPtr const &camera, math::matrix<4,4> const &tf,
	size_t ncols, size_t nrows, math::vec<3> const &scale)
:
	renderer_(renderer),
	scheduler_(scheduler),
	heightfield_(tf, nrows, ncols, scale)
{
	vertex_array_ = renderer->create_vertex_array<graphic::VERTEX_PNTB>(nrows * ncols);
	index_array_ = renderer->create_index_array<graphic::INDEX_U16>((nrows - 1) * (ncols - 1) * 2 * 3);
	material_ = renderer->create_material();
	draw_call_ = renderer->create_uber_draw_call();

	draw_call_->set_always_visible(true);
	draw_call_->set_priority(-2);
	draw_call_->set_vertex_buffer(vertex_array_->get_vertex_buffer());
	draw_call_->set_index_buffer(index_array_->get_index_buffer());
	draw_call_->set_material(material_);
	draw_call_->set_vertices_count(nrows * ncols);
	draw_call_->set_triangles_count((nrows - 1) * (ncols - 1) * 2);
	draw_call_->set_world_matrix(tf);
	draw_call_->set_camera(camera);
}

Heightfield::~Heightfield()
{
}

void Heightfield::set_texture(std::string const &filename, size_t index)
{
	material_->set_diffuse_texture(filename, index);
}

void Heightfield::update()
{
	{
		graphic::VERTEX_PNTB *vertex = vertex_array_->lock(true);

		for (size_t i = 0; i < heightfield_.ncols(); ++i)
		{
			for (size_t j = 0; j < heightfield_.nrows(); ++j)
			{
				math::scalar h = heightfield_.y_at(i, j);

				vertex->position.set(i, h, j);
				vertex->normal.set(0, 1, 0);
				vertex->texcoords.set(i, j);

				if (h < -1)
				{
					vertex->texutre_blend_weights.set(1, 0, 0, 0);
				}
				else if (h < 0)
				{
					vertex->texutre_blend_weights.set(-h, 1 + h, 0, 0);
				}
				else if (h < 1)
				{
					vertex->texutre_blend_weights.set(0, 1 - h, h, 0);
				}
				else
				{
					vertex->texutre_blend_weights.set(0, 0, 1, 0);
				}

				++vertex;
			}
		}

		vertex_array_->unlock();
	}

	{
		graphic::INDEX_U16 *index = index_array_->lock(true);

		for (size_t i = 1; i < heightfield_.ncols(); ++i)
		{
			for (size_t j = 1; j < heightfield_.nrows(); ++j)
			{
				*index++ = (i - 1) + (j - 1) * heightfield_.ncols();
				*index++ = (i - 0) + (j - 1) * heightfield_.ncols();
				*index++ = (i - 0) + (j - 0) * heightfield_.ncols();

				*index++ = (i - 1) + (j - 1) * heightfield_.ncols();
				*index++ = (i - 1) + (j - 0) * heightfield_.ncols();
				*index++ = (i - 0) + (j - 0) * heightfield_.ncols();
			}
		}

		index_array_->unlock();
	}
}

void Heightfield::draw()
{
	draw_call_->commit();
	scheduler_->add(draw_call_);
}

void Heightfield::bind(lua_State *L)
{
	using namespace luabind;

    module_(L, "pirates")
    [
        class_<Heightfield, boost::shared_ptr<Heightfield> >("Heightfield")
		.enum_("texture_type")
		[
			value("SEABED", TEXTURE_SEABED),
			value("SAND", TEXTURE_SAND),
			value("GRASS", TEXTURE_GRASS)
		]
		.def(constructor<graphic::RendererPtr const &, graphic::SchedulerPtr const &,
			graphic::CameraPtr const &, math::matrix<4,4> const &,
			size_t, size_t, math::vec<3> const &>())
		.def("update", &Heightfield::update)
		.def("draw", &Heightfield::draw)
		.def("set_texture", &Heightfield::set_texture)
	];
}

}

