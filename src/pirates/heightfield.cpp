//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2011 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <graphic/material.h>
#include <graphic/renderer.h>
#include "heightfield.h"

namespace pirates
{

Heightfield::Heightfield(graphic::RendererPtr const &renderer, math::matrix<4,4> const &tf,
	size_t ncols, size_t nrows, math::vec<3> const &scale)
:
	renderer_(renderer),
	heightfield_(tf, nrows, ncols, scale)
{
	vertex_array_ = renderer->create_vertex_array<graphic::VERTEX_PNTB>(nrows * ncols);
	index_array_ = renderer->create_index_array<graphic::INDEX_U16>((nrows - 1) * (ncols - 1) * 2);
	material_ = renderer->create_material();
	draw_call_ = renderer->create_uber_draw_call();
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
			}
		}
	}
}

void Heightfield::draw()
{
}

}

