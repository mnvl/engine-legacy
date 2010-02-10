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
#include <graphic/renderer.h>
#include <graphic/static_model.h>
#include <graphic/static_mesh.h>

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
	return (StaticModel *) model_ptr_;
}

template<class T>
void BasicStaticMesh<T>::draw(Joint *joint_ptr)
{
	HRESULT hr;

	boost::shared_ptr<Renderer> const renderer_ptr = get_model()->get_renderer();
    SharedResources *shared_resources_ptr = renderer_ptr->get_shared_resources();
	IDirect3DDevice9 *device_ptr = renderer_ptr->get_device();

	{
		math::matrix<4,4> tf = joint_ptr->matrix * renderer_ptr->get_world_matrix();
		hr = device_ptr->SetTransform(D3DTS_WORLD, (D3DMATRIX const *) tf.ij);
		if (FAILED(hr)) throw exception::directx_error(hr);
	}

	hr = device_ptr->SetVertexDeclaration(shared_resources_ptr->get_vertex_decl<vertex_t>());
	if (FAILED(hr)) throw exception::directx_error(hr);

    hr = device_ptr->SetStreamSource(0, vertex_buffer_->get(), 0, sizeof(vertex_t));
	if (FAILED(hr)) throw exception::directx_error(hr);

    hr = device_ptr->SetIndices(index_buffer_->get());
	if (FAILED(hr)) throw exception::directx_error(hr);

    draw_triangles_list();
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

    for (size_t i = 0; i < vertices_count_; i++, p++, buffer++)
    {
        p->position = *buffer;
    }
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
void BasicStaticMesh<vertex_pnt>::set_joint_indices(math::vec<8, unsigned> const *buffer)
{
    // not supported
}

template<>
void BasicStaticMesh<vertex_pnjwt>::set_joint_indices(math::vec<8, unsigned> const *buffer)
{
    vertex_t *p = vertex_buffer_->begin();

    for (size_t i = 0; i < vertices_count_; i++, p++, buffer++)
    {
        p->joints = *buffer;
    }
}

template<>
void BasicStaticMesh<vertex_pnt>::set_joint_weights(math::vec<8> const *buffer)
{
    // not supported
}

template<>
void BasicStaticMesh<vertex_pnjwt>::set_joint_weights(math::vec<8> const *buffer)
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

template<class T>
void BasicStaticMesh<T>::draw_triangles_list()
{
	HRESULT hr;

	boost::shared_ptr<Renderer> renderer_ptr = get_model()->get_renderer();
    SharedResources *shared_resources_ptr = renderer_ptr->get_shared_resources();
	IDirect3DDevice9 *device_ptr = renderer_ptr->get_device();

	for (triangles_list_type::const_iterator it = triangles_list.begin();
		it != triangles_list.end(); it++)
	{
		if (it->material_ptr)
		{
			it->material_ptr->select();
		}
		else
		{
			shared_resources_ptr->get_default_material()->select();
		}

		hr = device_ptr->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, it->minimal_vertex_index, it->vertices_count,
			it->first_index_offset, it->triangles_count);
		if (FAILED(hr)) throw exception::directx_error(hr);
	}
}

}
