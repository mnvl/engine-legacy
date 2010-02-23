//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <luabind/luabind.hpp>
#include <exception/directx_error.h>
#include "renderer.h"
#include "index_array.h"

namespace graphic
{

IndexBuffer::IndexBuffer(boost::shared_ptr<Renderer> const &renderer, D3DFORMAT format, size_t size,
	bool dynamic, bool writeonly)
:
	renderer_(renderer),
	format_(format),
	clone_(size),
	usage_(0),
	pool_(dynamic ? D3DPOOL_DEFAULT : D3DPOOL_MANAGED)
{
	if(dynamic) usage_ |= D3DUSAGE_DYNAMIC;
	if(writeonly) usage_ |= D3DUSAGE_WRITEONLY;

	IDirect3DIndexBuffer9 *buffer;
	HRESULT hr = renderer_->get_device()->CreateIndexBuffer(size, usage_, format_, pool_, &buffer, 0);
	if(FAILED(hr)) throw exception::directx_error(hr);

	buffer_.reset(buffer);
	buffer->Release();
}

IndexBuffer::~IndexBuffer()
{
}

const void *IndexBuffer::lock() const
{
	return &clone_[0];
}

void IndexBuffer::unlock() const
{
}

void *IndexBuffer::lock(bool discard)
{
	return &clone_[0];
}

void IndexBuffer::unlock()
{
	void *ptr = 0;

	HRESULT hr = buffer_->Lock(0, 0, (void **) &ptr, (usage_ & D3DUSAGE_DYNAMIC) ? D3DLOCK_DISCARD : 0);
	if(FAILED(hr)) throw exception::directx_error(hr);

	memcpy(ptr, &clone_[0], clone_.size());

	hr = buffer_->Unlock();
	if(FAILED(hr)) throw exception::directx_error(hr);
}

void IndexBuffer::handle_lost_device()
{
	if (pool_ == D3DPOOL_MANAGED) return;

	buffer_.reset();
}

void IndexBuffer::handle_reset_device()
{
	if (pool_ == D3DPOOL_MANAGED) return;

	IDirect3DIndexBuffer9 *buffer;
	HRESULT hr = renderer_->get_device()->CreateIndexBuffer(clone_.size(), usage_, format_, pool_, &buffer, 0);
	if(FAILED(hr)) throw exception::directx_error(hr);

	buffer_.reset(buffer);
	buffer->Release();

	// unlock will restore contents of buffer
	lock();
	unlock();
}

void IndexBuffer::bind(lua_State *L)
{
	using namespace luabind;

	module_(L, "graphic")
	[
		class_<IndexBuffer, Resource, boost::shared_ptr<IndexBuffer> >("IndexBuffer")
	];
}

}
