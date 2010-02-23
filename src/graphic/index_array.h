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

#include "index_buffer.h"

namespace graphic
{

template <class T>
class IndexArray: public IndexBuffer
{
	friend Renderer;

public:
	typedef T index_t;

	index_t *lock(bool discard) { return (index_t *) IndexBuffer::lock(discard); }
	index_t const *lock() const { return (index_t const *) IndexBuffer::lock(); }

	index_t *begin() { return (index_t *) IndexBuffer::begin(); }
	index_t const *begin() const { return (index_t const *) IndexBuffer::begin(); }

	index_t *at(size_t index) { return &begin()[index]; }
	index_t const *at(size_t index) const { return &begin()[index]; }

protected:
	IndexArray(boost::shared_ptr<Renderer> const &renderer, size_t count,
		bool dynamic = false, bool writeonly = false)
	:
		IndexBuffer(renderer, INDEX_FORMAT, count * sizeof(index_t), dynamic, writeonly)
	{
	}
};

typedef IndexArray<index_t> IndexArray16;

void bind_index_array(lua_State *L);

}
