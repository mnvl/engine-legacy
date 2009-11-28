#pragma once

#include <lua.h>
#include <boost/shared_ptr.hpp>
#include <boost/intrusive_ptr.hpp>
#include <util/boost_com_intrusive_ptr.h>
#include "vertex.h"

namespace graphic
{

class Renderer;

class VertexBuffer
{
public:
	VertexBuffer(boost::shared_ptr<Renderer> const &renderer, size_t size,
		bool dynamic = false, bool writeonly = false);
	~VertexBuffer();

	void *lock();
	const void *lock() const;
	void unlock() const;

	void *begin() { return begin_; }
	void const *begin() const { return begin_; }

	IDirect3DVertexBuffer9 *get() const { return buffer_.get(); }

	virtual IDirect3DVertexDeclaration9 *get_vertex_decl() const = 0;
	virtual size_t get_vertex_size() const = 0;

private:
	boost::intrusive_ptr<IDirect3DVertexBuffer9> buffer_;
	void *begin_;

protected:
	boost::shared_ptr<Renderer> const &renderer_;
};

template <class T>
class VertexArray: public VertexBuffer {
public:
	typedef T vertex_t;

	VertexArray(boost::shared_ptr<Renderer> const &renderer, size_t count,
		bool dynamic = false, bool writeonly = false)
	:
		VertexBuffer(renderer, count * sizeof(vertex_t), dynamic, writeonly)
	{
	}

	vertex_t *lock() { return (vertex_t *) VertexBuffer::lock(); }
	vertex_t const *lock() const { return (vertex_t const *) VertexBuffer::lock(); }

	vertex_t *begin() { return (vertex_t *) VertexBuffer::begin(); }
	vertex_t const *begin() const { return (vertex_t const *) VertexBuffer::begin(); }

	vertex_t *at(size_t index) { return &begin()[index]; }
	vertex_t const *at(size_t index) const { return &begin()[index]; }

	virtual IDirect3DVertexDeclaration9 *get_vertex_decl() const;
	virtual size_t get_vertex_size() const;
};

typedef VertexArray<vertex_pnt> VertexArrayPNT;
typedef VertexArray<vertex_pnjwt> VertexArrayPNJWT;

void bind_vertex_array(lua_State *L);

}
