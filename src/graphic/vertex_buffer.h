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
#include <luabind/lua_include.hpp>
#include <util/boost_com_intrusive_ptr.h>
#include "vertex.h"
#include "vertex_traits.h"
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

	size_t get_size() const { return clone_.size(); }

	void *begin() { return &clone_[0]; }
	void const *begin() const { return &clone_[0]; }

	IDirect3DVertexBuffer9 *get() const { return buffer_.get(); }

	virtual VertexTraits const *get_vertex_traits() const = 0;
	virtual IDirect3DVertexDeclaration9 *get_vertex_decl() const = 0;

	virtual void handle_lost_device();
	virtual void handle_reset_device();

	static void bind(lua_State *L);

protected:
	boost::shared_ptr<Renderer> renderer_;
	boost::intrusive_ptr<IDirect3DVertexBuffer9> buffer_;
	std::vector<char> clone_;
	DWORD usage_;
	D3DPOOL pool_;

	VertexBuffer(boost::shared_ptr<Renderer> const &renderer, size_t size,
		bool dynamic = false, bool writeonly = false);
};

}
