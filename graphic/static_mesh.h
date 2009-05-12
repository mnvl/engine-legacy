#pragma once

#include <graphic/mesh.h>
#include <graphic/memory_mesh.h>
#include <util/boost_com_intrusive_ptr.h>

namespace graphic {

class static_model;

class static_mesh: public mesh {
public:
	static DWORD const VERTEX_FORMAT = memory_mesh::VERTEX_FORMAT;
	static D3DFORMAT const INDEX_FORMAT = memory_mesh::INDEX_FORMAT;
	typedef memory_mesh::vertex_t vertex_t;
	typedef memory_mesh::index_t index_t;

	static_mesh(static_model *model_ptr);
	~static_mesh();

	virtual static_model *get_model();
	virtual void draw();

private:
	boost::intrusive_ptr<IDirect3DVertexBuffer9> vertex_buffer_;
	boost::intrusive_ptr<IDirect3DIndexBuffer9> index_buffer_;
};

};