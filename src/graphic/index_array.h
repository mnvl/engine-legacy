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

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/intrusive_ptr.hpp>
#include <luabind/lua_include.hpp>
#include <util/boost_com_intrusive_ptr.h>
#include "index.h"
#include "resource.h"

namespace graphic
{

class Renderer;

class IndexBuffer: public Resource
{
	friend Renderer;

public:
	virtual ~IndexBuffer();

	const void *lock() const;
	void unlock() const;
	void *lock(bool discard);
	void unlock();

	void *begin() { return &clone_[0]; }
	void const *begin() const { return &clone_[0]; }

	IDirect3DIndexBuffer9 *get() const { return buffer_.get(); }

	virtual void handle_lost_device();
	virtual void handle_reset_device();

protected:
	boost::shared_ptr<Renderer> renderer_;

	IndexBuffer(boost::shared_ptr<Renderer> const &renderer, D3DFORMAT format, size_t size,
		bool dynamic = false, bool writeonly = false);

private:
	D3DFORMAT format_;
	boost::intrusive_ptr<IDirect3DIndexBuffer9> buffer_;
	std::vector<char> clone_;
	DWORD usage_;
	D3DPOOL pool_;
};

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
