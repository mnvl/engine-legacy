#pragma once
//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include "vertex_buffer.h"

namespace graphic
{

class Renderer;

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

	vertex_t *end() { return (vertex_t *) VertexBuffer::end(); }
	vertex_t const *end() const { return (vertex_t const *) VertexBuffer::end(); }

	vertex_t *at(size_t index) { return &begin()[index]; }
	vertex_t const *at(size_t index) const { return &begin()[index]; }

	virtual VertexTraits const *get_vertex_traits() const;
	virtual IDirect3DVertexDeclaration9 *get_vertex_decl() const;

	size_t get_count() const { return get_size() / sizeof(T); }

protected:
	VertexArray(boost::shared_ptr<Renderer> const &renderer, size_t count, bool dynamic = false, bool writeonly = false):
		VertexBuffer(renderer, count * sizeof(vertex_t), dynamic, writeonly)
	{
	}
};

typedef VertexArray<VERTEX_PT> VertexArrayPT;
typedef VertexArray<VERTEX_PNT> VertexArrayPNT;
typedef VertexArray<VERTEX_PTBNT> VertexArrayPTBNT;
typedef VertexArray<VERTEX_PNJWT> VertexArrayPNJWT;
typedef VertexArray<VERTEX_PTBNJWT> VertexArrayPTBNJWT;

void bind_vertex_array(lua_State *L);

}
