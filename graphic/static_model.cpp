
#include <boost/lambda/lambda.hpp>
#include <exception/directx_error.h>
#include <graphic/static_model.h>

namespace engine {
namespace graphic {

static_model::static_model(renderer *renderer_ptr):
renderer_ptr_(renderer_ptr)
{
}

static_model::~static_model()
{
}

void static_model::build_from_memory_model(const engine::graphic::memory_model *model_ptr)
{
	size_t total_vertices = 0, total_indices = 0;

	for (memory_model::meshes_type::const_iterator it = model_ptr->meshes.begin();
		it != model_ptr->meshes.end(); it++)
	{
		total_vertices = it->vertices.size();
		total_indices = it->indices.size();
	}

	HRESULT hr;

	{
		IDirect3DVertexBuffer9 *vb;
		hr = renderer_ptr_->get_device()->CreateVertexBuffer(total_vertices, 0, VERTEX_FORMAT,
			D3DPOOL_MANAGED, &vb, 0);
		if (FAILED(hr)) throw exception::directx_error(hr);
		vertex_buffer_.reset(vb);
		vb->Release();
	}

	{
		IDirect3DIndexBuffer9 *ib;
		hr = renderer_ptr_->get_device()->CreateIndexBuffer(total_indices, 0, INDEX_FORMAT,
			D3DPOOL_MANAGED, &ib, 0);
		if (FAILED(hr)) throw exception::directx_error(hr);
		index_buffer_.reset(ib);
		ib->Release();
	}

	vertex_t *vertex_put;
	index_t *index_put;
	size_t index_offset = 0;

	hr = vertex_buffer_->Lock(0, 0, (void **) &vertex_put, D3DLOCK_DISCARD);
	if (FAILED(hr)) throw exception::directx_error(hr);

	hr = index_buffer_->Lock(0, 0, (void **) &index_put, D3DLOCK_DISCARD);
	if (FAILED(hr)) throw exception::directx_error(hr);

	for (memory_model::meshes_type::const_iterator it = model_ptr->meshes.begin();
		it != model_ptr->meshes.end(); it++)
	{
		std::vector<vertex_t> buffer;
		it->transform_vertices(buffer);
		vertex_put = std::copy(buffer.begin(), buffer.end(), vertex_put);

		using namespace boost::lambda;
		index_put = std::transform(it->indices.begin(), it->indices.end(), index_put, _1 + index_offset);
	}

	vertex_buffer_->Unlock();
	index_buffer_->Unlock();
}

}
}