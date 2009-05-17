
#include <exception/directx_error.h>
#include <graphic/static_model.h>
#include <graphic/static_mesh.h>

namespace graphic
{

template static_mesh;
template pnjwt_static_mesh;

template<class T>
basic_static_mesh<T>::basic_static_mesh(static_model *model_ptr):
    mesh(model_ptr),
    vertex_buffer_locked_(0),
    index_buffer_locked_(0)
{
}

template<class T>
basic_static_mesh<T>::~basic_static_mesh()
{
}

template<class T>
static_model *basic_static_mesh<T>::get_model()
{
	return (static_model *) model_ptr_;
}

template<class T>
void basic_static_mesh<T>::draw()
{
}

template<class T>
void basic_static_mesh<T>::set_vertices_count(size_t count)
{
    IDirect3DVertexBuffer *vb;
    HRESULT hr = get_model()->get_device()->CreateVertexBuffer(count * sizeof(vertex_t), 0, 0, D3DPOOL_MANAGED, &vb, 0);
    if (FAILED(hr)) throw exception::directx_error(hr);
    vertex_buffer_ptr_.reset(vb);
    vb->Release();

    vertices_count_ = count;
}

template<class T>
size_t basic_static_mesh<T>::get_vertices_count()
{
	return vertices_count_;
}

template<class T>
void basic_static_mesh<T>::set_indices_count(size_t count)
{
    IDirect3DIndexBuffer *vb;
    HRESULT hr = get_model()->get_device()->CreateIndexBuffer(count * sizeof(index_t), 0, D3DFMT_INDEX32, D3DPOOL_MANAGED, &ib, 0);
    if (FAILED(hr)) throw exception::directx_error(hr);
    index_buffer_ptr_.reset(vb);
    ib->Release();

    indices_count_ = count;
}

template<class T>
void basic_static_mesh<T>::lock()
{
    HRESULT hr;

    hr = vertex_buffer_ptr_->Lock(0, 0, &vertex_buffer_locked_, 0);
    if (FAILED(hr)) throw exception::directx_error(hr);

    hr = index_buffer_ptr_->Lock(0, 0, &index_buffer_locked_, 0);
    if (FAILED(hr)) throw exception::directx_error(hr);
}

template<class T>
void basic_static_mesh<T>::set_positions(math::vec<3> const *buffer)
{
    vertex_t *p = vertex_buffer_locked_;

    for (size_t i = 0; i < vertices_count_; i++, p++, buffer++)
    {
        p->position = *buffer;
    }
}

template<class T>
void basic_static_mesh<T>::set_normals(math::vec<3> const *buffer)
{
    vertex_t *p = vertex_buffer_locked_;

    for (size_t i = 0; i < vertices_count_; i++, p++, buffer++)
    {
        p->normals = *buffer;
    }
}

template<>
void basic_static_mesh<vertex_pnt>::set_joint_indices(math::vec<8, unsigned> const *buffer)
{
    // not supported
}

template<>
void basic_static_mesh<vertex_pnjwt>::set_joint_indices(math::vec<8, unsigned> const *buffer)
{
    vertex_t *p = vertex_buffer_locked_;

    for (size_t i = 0; i < vertices_count_; i++, p++, buffer++)
    {
        p->joints = *buffer;
    }
}

template<>
void basic_static_mesh<vertex_pnt>::set_joint_weights(math::vec<8> const *buffer)
{
    // not supported
}

template<>
void basic_static_mesh<vertex_pnjwt>::set_joint_weights(math::vec<8> const *buffer)
{
    vertex_t *p = vertex_buffer_locked_;

    for (size_t i = 0; i < vertices_count_; i++, p++, buffer++)
    {
        p->weights = *buffer;
    }
}

template<class T>
void basic_static_mesh<T>::set_texcoords(math::vec<2> const *buffer)
{
    vertex_t *p = vertex_buffer_locked_;

    for (size_t i = 0; i < vertices_count_; i++, p++, buffer++)
    {
        p->texcoords = *buffer;
    }
}

template<class T>
void basic_static_mesh<T>::set_face_indices(unsigned *buffer)
{
    index_t *p = index_buffer_locked_;

    for (size_t i = 0; i < indices_count; i++, p++, buffer++)
    {
        *p = *buffer;
    }
}

template<class T>
void basic_static_mesh<T>::unlock()
{
    HRESULT hr;

    hr = vertex_buffer_ptr_->Unlock();
    if (FAILED(hr)) throw exception::directx_error(hr);
    vertex_buffer_locked_ = 0;

    hr = index_buffer_ptr->Unlock();
    if (FAILED(hr)) throw exception::directx_error(hr);
    index_buffer_locked_ = 0;
}

}
