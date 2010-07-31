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

#include "vertex.h"
#include "vertex_buffer.h"

namespace graphic
{

class Renderer;

template <class T>
class VertexArray {
public:
	typedef T vertex_type;

	VertexArray(boost::shared_ptr<VertexBuffer> const &vertex_buffer):
		vertex_buffer_(vertex_buffer)
	{
	}

	virtual ~VertexArray()
	{
	}

	vertex_type *lock(bool discard) { return (vertex_type *) vertex_buffer_->lock(discard); }
	vertex_type const *lock() const { return (vertex_type const *) vertex_buffer_->lock(); }

	void unlock() { vertex_buffer_->unlock(); }
	void unlock() const { vertex_buffer_->unlock(); }

	vertex_type *begin() { return (vertex_type *) vertex_buffer_->begin(); }
	vertex_type const *begin() const { return (vertex_type const *) vertex_buffer_->begin(); }

	vertex_type *end() { return (vertex_type *) vertex_buffer_->end(); }
	vertex_type const *end() const { return (vertex_type const *) vertex_buffer_->end(); }

	vertex_type *at(size_t index) { return begin() + index; }
	vertex_type const *at(size_t index) const { return begin() + index; }

	size_t get_size() const { return vertex_buffer_->get_size(); }
	size_t get_count() const { return get_size() / sizeof(T); }

	boost::shared_ptr<VertexBuffer> const &get_vertex_buffer() { return vertex_buffer_;	}

protected:
	boost::shared_ptr<VertexBuffer> vertex_buffer_;
};

typedef VertexArray<VERTEX_PT> VertexArrayPT;
typedef VertexArray<VERTEX_PNT> VertexArrayPNT;
typedef VertexArray<VERTEX_PTBNT> VertexArrayPTBNT;
typedef VertexArray<VERTEX_PNJWT> VertexArrayPNJWT;
typedef VertexArray<VERTEX_PTBNJWT> VertexArrayPTBNJWT;

void bind_vertex_array(lua_State *L);

}
