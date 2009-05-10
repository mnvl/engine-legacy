#pragma once

#include <graphic/mesh.h>
#include <graphic/model.h>
#include <graphic/memory_model.h>
#include <graphic/renderer.h>
#include <util/boost_com_intrusive_ptr.h>

namespace graphic {

class static_model;

class static_mesh: public mesh {
public:
	static_mesh(static_model *model_ptr, size_t first_vertex_offset, size_t first_index_offset,
		size_t vertices_count, size_t indices_count);
	~static_mesh();

	virtual static_model *get_model();
	virtual void draw();

private:
	size_t first_vertex_offset_, first_index_offset_, vertices_count_, indices_count_;
};

class static_model: public model {
public:
	static DWORD const VERTEX_FORMAT = memory_mesh::VERTEX_FORMAT;
	static D3DFORMAT const INDEX_FORMAT = memory_mesh::INDEX_FORMAT;
	typedef memory_mesh::vertex_t vertex_t;
	typedef memory_mesh::index_t index_t;

	static_model(renderer *renderer_ptr);
	~static_model();

	void build_from_memory_model(memory_model *model_ptr);

	void draw();

private:
	boost::intrusive_ptr<IDirect3DVertexBuffer9> vertex_buffer_;
	boost::intrusive_ptr<IDirect3DIndexBuffer9> index_buffer_;
};

}

