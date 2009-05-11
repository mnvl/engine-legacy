
#include <boost/lambda/lambda.hpp>
#include <exception/directx_error.h>
#include <graphic/static_model.h>

namespace graphic {

static_mesh::static_mesh(static_model *model_ptr, size_t first_vertex_offset, size_t first_index_offset,
	size_t vertices_count, size_t indices_count):
mesh(model_ptr),
first_vertex_offset_(first_vertex_offset),
first_index_offset_(first_index_offset),
vertices_count_(vertices_count),
indices_count_(indices_count)
{
}

static_mesh::~static_mesh()
{
}

static_model *static_mesh::get_model()
{
	return (static_model *) model_ptr_;
}

void static_mesh::draw()
{
	HRESULT hr = get_model()->get_renderer()->get_device()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
		vertices_count_, first_vertex_offset_, indices_count_ / 3);
	if (FAILED(hr)) throw exception::directx_error(hr);
}

static_model::static_model(renderer *renderer_ptr):
model(renderer_ptr)
{
}

static_model::~static_model()
{
}

void static_model::build_from_memory_model(graphic::memory_model *model_ptr)
{
	size_t total_vertices = 0, total_indices = 0;

	for (memory_model::joints_type::const_iterator it = model_ptr->joints.begin();
		it != model_ptr->joints.end(); it++)
	{
		if (it->mesh_ptr)
		{
			memory_mesh *mesh_ptr = (memory_mesh *) it->mesh_ptr;

			std::auto_ptr<static_mesh> static_mesh_ptr(new static_mesh(this, total_vertices, total_indices,
				mesh_ptr->vertices.size(), mesh_ptr->indices.size()));
			static_mesh_ptr->id = mesh_ptr->id;
			static_mesh_ptr->name = mesh_ptr->name;

			meshes.push_back(static_mesh_ptr);

			total_vertices += mesh_ptr->vertices.size();
			total_indices += mesh_ptr->indices.size();
		}
	}

	HRESULT hr;

	{
		IDirect3DVertexBuffer9 *vb;
		hr = renderer_ptr_->get_device()->CreateVertexBuffer(total_vertices * sizeof(vertex_t), 0, VERTEX_FORMAT,
			D3DPOOL_MANAGED, &vb, 0);
		if (FAILED(hr)) throw exception::directx_error(hr);
		vertex_buffer_.reset(vb);
		vb->Release();
	}

	{
		IDirect3DIndexBuffer9 *ib;
		hr = renderer_ptr_->get_device()->CreateIndexBuffer(total_indices * sizeof(index_t), 0, INDEX_FORMAT,
			D3DPOOL_MANAGED, &ib, 0);
		if (FAILED(hr)) throw exception::directx_error(hr);
		index_buffer_.reset(ib);
		ib->Release();
	}

	vertex_t *vertex_put;
	index_t *index_put;
	size_t index_offset = 0;

	hr = vertex_buffer_->Lock(0, 0, (void **) &vertex_put, 0);
	if (FAILED(hr)) throw exception::directx_error(hr);

	hr = index_buffer_->Lock(0, 0, (void **) &index_put, 0);
	if (FAILED(hr)) throw exception::directx_error(hr);

	model_ptr->set_animation_time(0);

	for (memory_model::joints_type::const_iterator it = model_ptr->joints.begin();
		it != model_ptr->joints.end(); it++)
	{
		if (it->mesh_ptr)
		{
			memory_mesh *mesh_ptr = (memory_mesh *) it->mesh_ptr;

			std::vector<vertex_t> buffer;
			mesh_ptr->transform_vertices(&*it, buffer);
			vertex_put = std::copy(buffer.begin(), buffer.end(), vertex_put);

			using namespace boost::lambda;
			index_put = std::transform(mesh_ptr->indices.begin(), mesh_ptr->indices.end(), index_put, _1 + index_offset);
			index_offset += mesh_ptr->indices.size();
		}
	}

	vertex_buffer_->Unlock();
	index_buffer_->Unlock();
}

void static_model::draw()
{
	HRESULT hr;
	IDirect3DDevice9 *device_ptr = renderer_ptr_->get_device();

	hr = device_ptr->SetFVF(VERTEX_FORMAT);
	if(FAILED(hr)) throw exception::directx_error(hr);

	hr = device_ptr->SetStreamSource(0, vertex_buffer_.get(), 0, sizeof(vertex_t));
	if(FAILED(hr)) throw exception::directx_error(hr);

	hr = device_ptr->SetIndices(index_buffer_.get());
	if(FAILED(hr)) throw exception::directx_error(hr);

	for (meshes_type::iterator it = meshes.begin(); it != meshes.end(); it++)
	{
		it->draw();
	}
}

}
