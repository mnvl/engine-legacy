#pragma once

#include <lua.h>
#include <boost/intrusive_ptr.hpp>
#include <util/boost_com_intrusive_ptr.h>
#include "vertex.h"

namespace graphic
{

class Renderer;

class VertexBuffer
{
public:
	VertexBuffer(Renderer *renderer, size_t size, bool dynamic = false, bool writeonly = false);
	~VertexBuffer();

	void *lock();
	const void *lock() const;
	void unlock() const;

	void *begin() { return begin_; }
	void const *begin() const { return begin_; }

private:
	boost::intrusive_ptr<IDirect3DVertexBuffer9> buffer_;
	void *begin_;
};

template <class T>
class VertexArray: public VertexBuffer {
public:
	typedef T vertex_t;
	typedef vertex_traits<vertex_t> traits_t;

	VertexArray(Renderer *renderer, size_t count, bool dynamic = false, bool writeonly = false):
		VertexBuffer(renderer, count * traits_t::get_size(), dynamic, writeonly)
	{
	}

	vertex_t *lock() { return (vertex_t *) VertexBuffer::lock(); }
	vertex_t const *lock() const { return (vertex_t const *) VertexBuffer::lock(); }

	vertex_t *begin() { return (vertex_t *) VertexBuffer::begin(); }
	vertex_t const *begin() const { return (vertex_t const *) VertexBuffer::begin(); }

	vertex_t *at(size_t index) { return &begin()[index]; }
	vertex_t const *at(size_t index) const { return &begin()[index]; }
};

typedef VertexArray<vertex_pnt> VertexArrayPNT;
typedef VertexArray<vertex_pnjwt> VertexArrayPNJWT;

void bind_vertex_array(lua_State *L);

}
