//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include "renderer.h"

namespace graphic
{

VertexDeclarations::VertexDeclarations(Renderer * const renderer_ptr):
	renderer_ptr_(renderer_ptr)
{
}

VertexDeclarations::~VertexDeclarations()
{
}

IDirect3DVertexDeclaration9 *VertexDeclarations::get(VertexTraits const *vertex_traits)
{
	traits_to_declarations_t::const_iterator it = traits_to_declarations_.find(vertex_traits);

	if (it == traits_to_declarations_.end())
	{
		IDirect3DVertexDeclaration9 *decl_ptr;

		HRESULT hr = renderer_ptr_->get_device()->CreateVertexDeclaration(vertex_traits->get_descr(), &decl_ptr);
		if (FAILED(hr)) throw exception::directx_error(hr);

		traits_to_declarations_.insert(std::make_pair(vertex_traits, VertexDeclarationPtr(decl_ptr)));

		decl_ptr->Release();

		return decl_ptr;
	}
	else
	{
		return it->second.get();
	}
}

}
