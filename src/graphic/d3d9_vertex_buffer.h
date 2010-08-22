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
#include "forward.h"
#include "vertex.h"
#include "vertex_buffer.h"
#include "vertex_traits.h"

namespace graphic
{

class Renderer;

class D3D9VertexBuffer: public VertexBuffer
{
	friend Renderer;

public:
	virtual ~D3D9VertexBuffer();

	const void *lock() const;
	void unlock() const;
	void *lock(bool discard);
	void unlock();

	size_t get_size() const { return clone_.size(); }

	void *begin() { return &clone_[0]; }
	void const *begin() const { return &clone_[0]; }
	void *end() { return &clone_[0] + get_size(); }
	void const *end() const { return &clone_[0] + get_size(); }

	IDirect3DVertexBuffer9 *get() const { return buffer_.get(); }

	VertexTraits const *get_vertex_traits() const { return traits_; }
	IDirect3DVertexDeclaration9 *get_vertex_decl() const { return decl_; }

	void handle_lost_device();
	void handle_reset_device();

protected:
	RendererPtr renderer_;
	boost::intrusive_ptr<IDirect3DVertexBuffer9> buffer_;
	std::vector<char> clone_;
	DWORD usage_;
	D3DPOOL pool_;
	VertexTraits const *traits_;
	IDirect3DVertexDeclaration9 *decl_;

	D3D9VertexBuffer(RendererPtr const &renderer, size_t size, bool dynamic, bool writeonly,
		VertexTraits const *traits, IDirect3DVertexDeclaration9 *decl);
};

}
