#pragma once

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/intrusive_ptr.hpp>
#include <lua.h>
#include <util/boost_com_intrusive_ptr.h>
#include "vertex.h"
#include "resource.h"

namespace graphic
{

class Renderer;

class VertexBuffer: public Resource
{
	friend Renderer;

public:
	virtual ~VertexBuffer();

	const void *lock() const;
	void unlock() const;
	void *lock(bool discard);
	void unlock();

	void *begin() { return &clone_[0]; }
	void const *begin() const { return &clone_[0]; }

	IDirect3DVertexBuffer9 *get() const { return buffer_.get(); }

	virtual IDirect3DVertexDeclaration9 *get_vertex_decl() const { return 0; }
	virtual size_t get_vertex_size() const { return 0; }

	virtual void handle_lost_device();
	virtual void handle_reset_device();

protected:
	boost::shared_ptr<Renderer> renderer_;

	VertexBuffer(boost::shared_ptr<Renderer> const &renderer, size_t size,
		bool dynamic = false, bool writeonly = false);

private:
	boost::intrusive_ptr<IDirect3DVertexBuffer9> buffer_;
	std::vector<char> clone_;
	DWORD usage_;
	D3DPOOL pool_;
};

template <class T>
class VertexArray: public VertexBuffer
{
	friend Renderer;

public:
	typedef T vertex_t;

	vertex_t *lock(bool discard) { return (vertex_t *) VertexBuffer::lock(discard); }
	vertex_t const *lock() const { return (vertex_t const *) VertexBuffer::lock(); }

	vertex_t *begin() { return (vertex_t *) VertexBuffer::begin(); }
	vertex_t const *begin() const { return (vertex_t const *) VertexBuffer::begin(); }

	vertex_t *at(size_t index) { return &begin()[index]; }
	vertex_t const *at(size_t index) const { return &begin()[index]; }

	virtual IDirect3DVertexDeclaration9 *get_vertex_decl() const;
	virtual size_t get_vertex_size() const;

protected:
	VertexArray(boost::shared_ptr<Renderer> const &renderer, size_t count,
		bool dynamic = false, bool writeonly = false)
	:
		VertexBuffer(renderer, count * sizeof(vertex_t), dynamic, writeonly)
	{
	}
};

typedef VertexArray<vertex_pt> VertexArrayPT;
typedef VertexArray<vertex_pnt> VertexArrayPNT;
typedef VertexArray<vertex_pnjwt> VertexArrayPNJWT;

void bind_vertex_array(lua_State *L);

}
