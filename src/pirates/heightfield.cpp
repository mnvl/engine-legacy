//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2011 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <graphic/uber_draw_call.h>
#include <graphic/material.h>
#include <graphic/renderer.h>
#include "heightfield.h"

namespace pirates
{

Heightfield::Heightfield(graphic::RendererPtr const &renderer, graphic::CameraPtr const &camera, math::matrix<4,4> const &tf,
	size_t ncols, size_t nrows, math::vec<3> const &scale)
:
	renderer_(renderer),
	heightfield_(tf, nrows, ncols, scale)
{
	vertex_array_ = renderer->create_vertex_array<graphic::VERTEX_PNTB>(nrows * ncols);
	index_array_ = renderer->create_index_array<graphic::INDEX_U16>((nrows - 1) * (ncols - 1) * 2);
	material_ = renderer->create_material();
	draw_call_ = renderer->create_uber_draw_call();

	draw_call_->set_always_visible(true);
	draw_call_->set_vertex_buffer(vertex_array_->get_vertex_buffer());
	draw_call_->set_index_buffer(index_array_->get_index_buffer());
	draw_call_->set_material(material_);
	draw_call_->set_vertices_count(nrows * ncols);
	draw_call_->set_triangles_count((nrows - 1) * (ncols - 1));
	draw_call_->set_world_matrix(tf);
	draw_call_->set_camera(camera);
	draw_call_->commit();
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
				math::scalar h = heightfield_.at(i, j);

				vertex->position.set(i, h, j);
				vertex->normal.set(0, 1, 0);
				vertex->texcoords.set(i, j);

				if (h < -1)
				{
					vertex->texutre_blend_weights.set(1, 0, 0, 0);
				}
				else if (h > 1)
				{
					vertex->texutre_blend_weights.set(0, 0, 1, 0);
				}
				else if (h > 0.5f)
				{
					vertex->texutre_blend_weights.set(0, 1 - h, h, 0);
				}
				else
				{
					math::scalar h2 = (h + 1) / 1.5f;
					vertex->texutre_blend_weights.set(1 - h2, h2, 0, 0);
				}

				++vertex;
			}
		}
	}

	{
		graphic::INDEX_U16 *index = index_array_->lock(true);


		for (size_t i = 1; i < heightfield_.ncols(); ++i)
		{
			for (size_t j = 1; j < heightfield_.nrows(); ++j)
			{
				*index++ = (i - 1) + (j - 1) * heightfield_.nrows();
				*index++ = (i - 0) + (j - 1) * heightfield_.nrows();
				*index++ = (i - 0) + (j - 0) * heightfield_.nrows();

				*index++ = (i - 1) + (j - 1) * heightfield_.nrows();
				*index++ = (i - 0) + (j - 0) * heightfield_.nrows();
				*index++ = (i - 1) + (j - 0) * heightfield_.nrows();
			}
		}
	}
}

void Heightfield::draw()
{
	draw_call_->execute();
}

}

