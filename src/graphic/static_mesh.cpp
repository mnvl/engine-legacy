
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
    Mesh(model_ptr),
    vertex_buffer_locked_(0),
    index_buffer_locked_(0)
{
}

template<class T>
BasicStaticMesh<T>::~BasicStaticMesh()
{
}

template<class T>
StaticModel *BasicStaticMesh<T>::get_model()
{
	return (StaticModel *) model_ptr_;
}

template<class T>
void BasicStaticMesh<T>::draw(Joint *joint_ptr)
{
	boost::shared_ptr<Renderer> const renderer_ptr = get_model()->get_renderer();
    ResourceManager *resource_manager_ptr = renderer_ptr->get_resource_manager();
	IDirect3DDevice9 *device_ptr = renderer_ptr->get_device();

	{
		math::matrix<4,4> tf = joint_ptr->matrix * renderer_ptr->get_world_matrix();
		device_ptr->SetTransform(D3DTS_WORLD, (D3DMATRIX const *) tf.ij);
	}

	device_ptr->SetVertexDeclaration(resource_manager_ptr->get_vertex_decl<vertex_t>());
    device_ptr->SetStreamSource(0, vertex_buffer_.get(), 0, sizeof(vertex_t));

    device_ptr->SetIndices(index_buffer_.get());

    draw_triangles_list();
}

template<class T>
void BasicStaticMesh<T>::set_vertices_count(size_t count)
{
    IDirect3DVertexBuffer9 *vb;
    HRESULT hr = get_model()->get_renderer()->get_device()->CreateVertexBuffer(count * sizeof(vertex_t), 0, 0, D3DPOOL_MANAGED, &vb, 0);
    if (FAILED(hr)) throw exception::directx_error(hr);
    vertex_buffer_.reset(vb);
    vb->Release();

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
    IDirect3DIndexBuffer9 *ib;
    HRESULT hr = get_model()->get_renderer()->get_device()->CreateIndexBuffer(count * sizeof(index_t), 0, INDEX_FORMAT, D3DPOOL_MANAGED, &ib, 0);
    if (FAILED(hr)) throw exception::directx_error(hr);
    index_buffer_.reset(ib);
    ib->Release();

    indices_count_ = count;
}

template<class T>
void BasicStaticMesh<T>::lock()
{
    HRESULT hr;

    hr = vertex_buffer_->Lock(0, 0, (void **) &vertex_buffer_locked_, 0);
    if (FAILED(hr)) throw exception::directx_error(hr);

    hr = index_buffer_->Lock(0, 0, (void **) &index_buffer_locked_, 0);
    if (FAILED(hr)) throw exception::directx_error(hr);
}

template<class T>
void BasicStaticMesh<T>::set_positions(math::vec<3> const *buffer)
{
    vertex_t *p = vertex_buffer_locked_;

    for (size_t i = 0; i < vertices_count_; i++, p++, buffer++)
    {
        p->position = *buffer;
    }
}

template<class T>
void BasicStaticMesh<T>::set_normals(math::vec<3> const *buffer)
{
    vertex_t *p = vertex_buffer_locked_;

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
    vertex_t *p = vertex_buffer_locked_;

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
    vertex_t *p = vertex_buffer_locked_;

    for (size_t i = 0; i < vertices_count_; i++, p++, buffer++)
    {
        p->weights = *buffer;
    }
}

template<class T>
void BasicStaticMesh<T>::set_texcoords(math::vec<2> const *buffer)
{
    vertex_t *p = vertex_buffer_locked_;

    for (size_t i = 0; i < vertices_count_; i++, p++, buffer++)
    {
        p->texcoords = *buffer;
    }
}

template<class T>
void BasicStaticMesh<T>::set_face_indices(unsigned *buffer)
{
    index_t *p = index_buffer_locked_;

    for (size_t i = 0; i < indices_count_; i++, p++, buffer++)
    {
        *p = *buffer;
    }
}

template<class T>
void BasicStaticMesh<T>::unlock()
{
    HRESULT hr;

    hr = vertex_buffer_->Unlock();
    if (FAILED(hr)) throw exception::directx_error(hr);
    vertex_buffer_locked_ = 0;

    hr = index_buffer_->Unlock();
    if (FAILED(hr)) throw exception::directx_error(hr);
    index_buffer_locked_ = 0;
}

template<class T>
void BasicStaticMesh<T>::draw_triangles_list()
{
	boost::shared_ptr<Renderer> renderer_ptr = get_model()->get_renderer();
    ResourceManager *resource_manager_ptr = renderer_ptr->get_resource_manager();
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
			resource_manager_ptr->get_default_material()->select();
		}

		device_ptr->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, it->minimal_vertex_index, it->vertices_count,
			it->first_index_offset, it->triangles_count);
	}
}

}
