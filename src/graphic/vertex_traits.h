#pragma once
//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2011 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <d3d9.h>

namespace graphic
{

class VertexTraits {
public:
	virtual size_t get_size() const = 0;
	virtual D3DVERTEXELEMENT9 const *get_descr() const = 0;

	template<class V>
	static VertexTraits const *get();
};

}

