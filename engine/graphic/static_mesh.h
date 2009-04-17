#pragma once

#include <d3d9.h>
#include <boost/intrusive_ptr.hpp>
#include <engine/util/boost_com_intrusive_ptr.h>
#include <engine/graphic/mesh.h>

namespace engine {
namespace graphic {

class renderer;

// после изменения размеров буферов данные в них портятся!
class static_mesh: public IMesh {
public:
	static_mesh(renderer *renderer_ptr);
	virtual ~static_mesh();

	virtual size_t get_vertices_count();
	virtual void resize_vertex_buffer(size_t size);
	virtual vertex *lock_vertex_buffer();
	virtual void unlock_vertex_buffer(vertex *buffer);

	virtual size_t get_indices_count();
	virtual void resize_index_buffer(size_t size);
	virtual index *lock_index_buffer();
	virtual void unlock_index_buffer(index *buffer);

private:
	renderer *renderer_ptr_;
	boost::intrusive_ptr<IDirect3DIndexBuffer9> index_buffer_ptr_;
	boost::intrusive_ptr<IDirect3DVertexBuffer9> vertex_buffer_ptr_;
};

}
}