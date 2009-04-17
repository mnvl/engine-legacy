
#include <engine/exception/directx_error.h>
#include <engine/graphic/renderer.h>
#include <engine/graphic/static_mesh.h>

namespace engine {
namespace graphic {

static_mesh::static_mesh(renderer *renderer_ptr):
renderer_ptr_(renderer_ptr)
{
}

static_mesh::~static_mesh()
{
}

size_t static_mesh::get_vertices_count()
{
	if (vertex_buffer_ptr_)
	{
		D3DVERTEXBUFFER_DESC descr;
		HRESULT hr = vertex_buffer_ptr_->GetDesc(&descr);
		if (FAILED(hr)) throw exception::directx_error(hr);
		return descr.Size;
	}

	return 0;
}

void static_mesh::resize_vertex_buffer(size_t size)
{
	IDirect3DVertexBuffer9 *vertex_buffer_ptr;
	HRESULT hr = renderer_ptr_->get_device()->CreateVertexBuffer(size, 0, VERTEX_FORMAT, D3DPOOL_DEFAULT,
		&vertex_buffer_ptr, 0);
	vertex_buffer_ptr_.reset(vertex_buffer_ptr);
	vertex_buffer_ptr->Release();
}

IMesh::vertex *static_mesh::lock_vertex_buffer()
{
	void *data;
	HRESULT hr = vertex_buffer_ptr_->Lock(0, 0, &data, 0);
	if (FAILED(hr)) throw exception::directx_error(hr);
	return (vertex *) data;
}

void static_mesh::unlock_vertex_buffer(vertex *buffer)
{
	HRESULT hr = vertex_buffer_ptr_->Unlock();
	if (FAILED(hr)) throw exception::directx_error(hr);
}

size_t static_mesh::get_indices_count()
{
	if (index_buffer_ptr_)
	{
		D3DINDEXBUFFER_DESC descr;
		HRESULT hr = index_buffer_ptr_->GetDesc(&descr);
		if (FAILED(hr)) throw exception::directx_error(hr);
		return descr.Size;
	}

	return 0;
}

void static_mesh::resize_index_buffer(size_t size)
{
	IDirect3DIndexBuffer9 *index_buffer_ptr;
	HRESULT hr = renderer_ptr_->get_device()->CreateIndexBuffer(size, 0, INDEX_FORMAT, D3DPOOL_DEFAULT,
		&index_buffer_ptr, 0);
	index_buffer_ptr_.reset(index_buffer_ptr);
	index_buffer_ptr->Release();
}

IMesh::index *static_mesh::lock_index_buffer()
{
	void *data;
	HRESULT hr = index_buffer_ptr_->Lock(0, 0, &data, 0);
	if (FAILED(hr)) throw exception::directx_error(hr);
	return (index *) data;
}

void static_mesh::unlock_index_buffer(index *buffer)
{
	HRESULT hr = index_buffer_ptr_->Unlock();
	if (FAILED(hr)) throw exception::directx_error(hr);
}

}
}