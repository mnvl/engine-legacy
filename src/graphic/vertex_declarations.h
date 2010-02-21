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

#include <map>
#include <d3d9.h>
#include <boost/intrusive_ptr.hpp>
#include <util/boost_com_intrusive_ptr.h>
#include "vertex.h"
#include "vertex_traits.h"

namespace graphic
{

class Renderer;

class VertexDeclarations {
public:
    VertexDeclarations(Renderer * const renderer_ptr);
    ~VertexDeclarations();

    template<class V> inline
    IDirect3DVertexDeclaration9 *get()
	{
		return get(VertexTraits::get<V>());
	}

	IDirect3DVertexDeclaration9 *get(VertexTraits const *vertex_traits);

private:
	Renderer * const renderer_ptr_;

	typedef boost::intrusive_ptr<IDirect3DVertexDeclaration9> VertexDeclarationPtr;
	typedef std::map<VertexTraits const *,  VertexDeclarationPtr> traits_to_declarations_t;

	traits_to_declarations_t traits_to_declarations_;
};

}
