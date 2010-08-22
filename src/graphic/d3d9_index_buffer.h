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

#include <d3d9.h>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/intrusive_ptr.hpp>
#include <util/boost_com_intrusive_ptr.h>
#include "index_buffer.h"

namespace graphic
{

class Renderer;

class D3D9IndexBuffer: public IndexBuffer
{
	friend Renderer;

public:
	virtual ~D3D9IndexBuffer();

	const void *lock() const;
	void unlock() const;
	void *lock(bool discard);
	void unlock();

	void *begin() { return &clone_[0]; }
	void const *begin() const { return &clone_[0]; }

	IDirect3DIndexBuffer9 *get() const { return buffer_.get(); }

	void handle_lost_device();
	void handle_reset_device();

protected:
	RendererPtr renderer_;

	D3D9IndexBuffer(RendererPtr const &renderer, D3DFORMAT format, size_t size,
		bool dynamic = false, bool writeonly = false);

private:
	D3DFORMAT format_;
	boost::intrusive_ptr<IDirect3DIndexBuffer9> buffer_;
	std::vector<char> clone_;
	DWORD usage_;
	D3DPOOL pool_;
};

}
