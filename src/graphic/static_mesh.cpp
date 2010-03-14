//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <exception/directx_error.h>
#include <util/logger.h>
#include "mesh_shader.h"
#include "mesh_draw_call.h"
#include "static_model.h"
#include "renderer.h"
#include "scheduler.h"
#include "static_mesh.h"

namespace graphic
{

template<class T>
BasicStaticMesh<T>::BasicStaticMesh(Model *model_ptr):
    Mesh(model_ptr)
{
}

template<class T>
BasicStaticMesh<T>::~BasicStaticMesh()
{
}

template<class T>
MeshPtr BasicStaticMesh<T>::shallow_copy() const
{
	return MeshPtr(new BasicStaticMesh<T>(*this));
}

template<class T>
BasicStaticModel<T> *BasicStaticMesh<T>::get_model()
{
	return (BasicStaticModel<T> *) model_;
}

template<class T>
void BasicStaticMesh<T>::draw(Joint *joint_ptr)
{
	for (triangles_list_type::const_iterator it = triangles_list.begin(); it != triangles_list.end(); it++)
	{
		boost::shared_ptr<MeshDrawCall> draw_call(new MeshDrawCall(get_model()->get_renderer(), shader_));

		{
			math::obb<3> bounds(bounds_);
			bounds.transform(joint_ptr->matrix);
			draw_call->set_obb(bounds);
		}

		draw_call->set_vertex_buffer(vertex_buffer_);
		draw_call->set_index_buffer(index_buffer_);
		draw_call->set_material(it->material_ptr);
		draw_call->set_minimal_vertex_index(it->minimal_vertex_index);
		draw_call->set_vertices_count(vertex_buffer_->get_count());
		draw_call->set_first_index_offset(it->first_index_offset);
		draw_call->set_triangles_count(it->triangles_count);
		draw_call->set_world_matrix(joint_ptr->matrix * get_model()->get_renderer()->get_world_matrix());
		draw_call->commit();

		get_model()->get_scheduler()->add(draw_call);
	}
}

template<class T>
void BasicStaticMesh<T>::set_vertices(GenericVertex *vertices, GenericVertex::format_t format, size_t count)
{
	LOG_DEBUG("vertices: count = " << count << ", format = " << std::hex << format);

	if ((format & T::FORMAT) != T::FORMAT)
	{
		throw std::runtime_error("insufficient data");
	}

	vertex_buffer_ = get_model()->get_renderer()->create_vertex_array<vertex_t>(count, false, true);

	bounds_.lo = bounds_.hi = vertices->position;

    vertex_buffer_->lock(false);

    vertex_t *p = vertex_buffer_->begin();

    for (size_t i = 0; i < count; i++, p++, vertices++)
    {
		*p = *vertices;
		bounds_.extend(vertices->position);
    }

    vertex_buffer_->unlock();

	LOG_DEBUG("bounds = " << bounds_);
}

template<class T>
void BasicStaticMesh<T>::set_indices(unsigned *buffer, size_t count)
{
	LOG_DEBUG("indices: count = " << count);

	index_buffer_ = get_model()->get_renderer()->create_index_array<index_t>(count, false, true);

    index_buffer_->lock(false);

    index_t *p = index_buffer_->begin();

    for (size_t i = 0; i < count; i++, p++, buffer++)
    {
        *p = *buffer;
    }

    index_buffer_->unlock();
}

template StaticMeshPNT;
template StaticMeshPTBNT;
template StaticMeshPNJWT;
template StaticMeshPTBNJWT;

}
