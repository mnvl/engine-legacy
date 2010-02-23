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

template StaticMesh;

template PnjwtStaticMesh;

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
StaticModel *BasicStaticMesh<T>::get_model()
{
	return (StaticModel *) model_;
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
		draw_call->set_vertices_count(it->vertices_count);
		draw_call->set_first_index_offset(it->first_index_offset);
		draw_call->set_triangles_count(it->triangles_count);
		draw_call->set_world_matrix(joint_ptr->matrix);

		get_model()->get_scheduler()->add(draw_call);
	}
}

template<class T>
void BasicStaticMesh<T>::set_vertices_count(size_t count)
{
	vertex_buffer_ = get_model()->get_renderer()->create_vertex_array<vertex_t>(count, false, true);
    vertices_count_ = count;
}

template<class T>
size_t BasicStaticMesh<T>::get_vertices_count()
{
	return vertices_count_;
}

template<class T>
void BasicStaticMesh<T>::set_indices_count(size_t count)
{
	index_buffer_ = get_model()->get_renderer()->create_index_array<index_t>(count, false, true);
    indices_count_ = count;
}

template<class T>
void BasicStaticMesh<T>::lock()
{
    vertex_buffer_->lock(false);
    index_buffer_->lock(false);
}

template<class T>
void BasicStaticMesh<T>::set_positions(math::vec<3> const *buffer)
{
    vertex_t *p = (T *) vertex_buffer_->begin();

	bounds_.lo = bounds_.hi = *buffer;

    for (size_t i = 0; i < vertices_count_; i++, p++, buffer++)
    {
        p->position = *buffer;
		bounds_.extend(*buffer);
    }

	LOG_DEBUG("bounds = " << bounds_);
}

template<class T>
void BasicStaticMesh<T>::set_normals(math::vec<3> const *buffer)
{
    vertex_t *p = (T *) vertex_buffer_->begin();

    for (size_t i = 0; i < vertices_count_; i++, p++, buffer++)
    {
        p->normal = *buffer;
    }
}

template<>
void BasicStaticMesh<VERTEX_PNT>::set_joint_indices(math::vec<8, unsigned> const *buffer)
{
    // not supported
}

template<>
void BasicStaticMesh<VERTEX_PNJWT>::set_joint_indices(math::vec<8, unsigned> const *buffer)
{
    vertex_t *p = vertex_buffer_->begin();

    for (size_t i = 0; i < vertices_count_; i++, p++, buffer++)
    {
        p->joints = *buffer;
    }
}

template<>
void BasicStaticMesh<VERTEX_PNT>::set_joint_weights(math::vec<8> const *buffer)
{
    // not supported
}

template<>
void BasicStaticMesh<VERTEX_PNJWT>::set_joint_weights(math::vec<8> const *buffer)
{
    vertex_t *p = vertex_buffer_->begin();

    for (size_t i = 0; i < vertices_count_; i++, p++, buffer++)
    {
        p->weights = *buffer;
    }
}

template<class T>
void BasicStaticMesh<T>::set_texcoords(math::vec<2> const *buffer)
{
    vertex_t *p = (T *) vertex_buffer_->begin();

    for (size_t i = 0; i < vertices_count_; i++, p++, buffer++)
    {
        p->texcoords = *buffer;
    }
}

template<class T>
void BasicStaticMesh<T>::set_face_indices(unsigned *buffer)
{
    index_t *p = index_buffer_->begin();

    for (size_t i = 0; i < indices_count_; i++, p++, buffer++)
    {
        *p = *buffer;
    }
}

template<class T>
void BasicStaticMesh<T>::unlock()
{
    vertex_buffer_->unlock();
    index_buffer_->unlock();
}

}
