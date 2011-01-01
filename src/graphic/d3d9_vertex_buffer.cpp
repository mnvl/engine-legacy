//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2011 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <exception/directx_error.h>
#include "renderer.h"
#include "vertex_declarations.h"
#include "d3d9_vertex_buffer.h"

namespace graphic
{

D3D9VertexBuffer::D3D9VertexBuffer(RendererPtr const &renderer, size_t size, bool dynamic, bool writeonly,
	VertexTraits const *traits, IDirect3DVertexDeclaration9 *decl)
:
	renderer_(renderer),
	clone_(size),
	usage_(0),
	pool_(dynamic ? D3DPOOL_DEFAULT : D3DPOOL_MANAGED),
	traits_(traits),
	decl_(decl)
{
	if(dynamic) usage_ |= D3DUSAGE_DYNAMIC;
	if(writeonly) usage_ |= D3DUSAGE_WRITEONLY;

	IDirect3DVertexBuffer9 *buffer;
	HRESULT hr = renderer_->get_device()->CreateVertexBuffer(size, usage_, 0, pool_, &buffer, 0);
	if(FAILED(hr)) throw exception::directx_error(hr);

	buffer_.reset(buffer);
	buffer->Release();
}

D3D9VertexBuffer::~D3D9VertexBuffer()
{
}

const void *D3D9VertexBuffer::lock() const
{
	return &clone_[0];
}

void D3D9VertexBuffer::unlock() const
{
}

void *D3D9VertexBuffer::lock(bool discard)
{
	return &clone_[0];
}

void D3D9VertexBuffer::unlock()
{
	if (!buffer_) return;

	void *ptr = 0;

	HRESULT hr = buffer_->Lock(0, 0, (void **) &ptr, (usage_ & D3DUSAGE_DYNAMIC) ? D3DLOCK_DISCARD : 0);
	if(FAILED(hr)) throw exception::directx_error(hr);

	memcpy(ptr, &clone_[0], clone_.size());

	hr = buffer_->Unlock();
	if(FAILED(hr)) throw exception::directx_error(hr);
}

void D3D9VertexBuffer::handle_lost_device()
{
	if (pool_ == D3DPOOL_MANAGED) return;

	buffer_.reset();
}

void D3D9VertexBuffer::handle_reset_device()
{
	if (pool_ == D3DPOOL_MANAGED) return;

	IDirect3DVertexBuffer9 *buffer;
	HRESULT hr = renderer_->get_device()->CreateVertexBuffer(clone_.size(), usage_, 0, pool_, &buffer, 0);
	if(FAILED(hr)) throw exception::directx_error(hr);

	buffer_.reset(buffer);
	buffer->Release();

	// unlock will restore contents of buffer
	lock();
	unlock();
}

}
