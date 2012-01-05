#pragma once
//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2012 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <boost/shared_ptr.hpp>
#include "forward.h"
#include "index.h"
#include "index_buffer.h"

namespace graphic
{

template <class T>
class IndexArray {
public:
	typedef T index_type;

	IndexArray(IndexBufferPtr const &index_buffer):
		index_buffer_(index_buffer)
	{
	}

	~IndexArray()
	{
	}

	index_type *lock(bool discard) { return (index_type *) index_buffer_->lock(discard); }
	index_type const *lock() const { return (index_type const *) index_buffer_->lock(); }

	void unlock() { index_buffer_->unlock(); }
	void unlock() const { index_buffer_->unlock(); }

	index_type *begin() { return (index_type *) index_buffer_->begin(); }
	index_type const *begin() const { return (index_type const *) index_buffer_->begin(); }

	index_type *at(size_t index) { return &begin()[index]; }
	index_type const *at(size_t index) const { return &begin()[index]; }

	void put(size_t index, index_type value) { *at(index) = value; }
	index_type get(size_t index) const { return *at(index); }

	IndexBufferPtr const &get_index_buffer() { return index_buffer_; }

protected:
	IndexBufferPtr const index_buffer_;
};

typedef IndexArray<INDEX_U16> IndexArray16;

void bind_index_array(lua_State *L);

}
