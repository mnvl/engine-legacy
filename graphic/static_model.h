#pragma once

#include <graphic/model.h>
#include <graphic/memory_model.h>
#include <graphic/renderer.h>
#include <util/boost_com_intrusive_ptr.h>

namespace engine {
namespace graphic {

class static_model: public model {
public:
	static DWORD const VERTEX_FORMAT = memory_mesh::VERTEX_FORMAT;
	static D3DFORMAT const INDEX_FORMAT = memory_mesh::INDEX_FORMAT;
	typedef memory_mesh::vertex_t vertex_t;
	typedef memory_mesh::index_t index_t;

	static_model(renderer *renderer_ptr);
	~static_model();

	void build_from_memory_model(memory_model const *model_ptr);

private:
	renderer *renderer_ptr_;

	boost::intrusive_ptr<IDirect3DVertexBuffer9> vertex_buffer_;
	boost::intrusive_ptr<IDirect3DIndexBuffer9> index_buffer_;
};

}
}
