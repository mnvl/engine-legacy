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

namespace boost {
	inline void intrusive_ptr_add_ref(IUnknown *unknown) { unknown->AddRef(); }
	inline void intrusive_ptr_release(IUnknown *unknown) { unknown->Release(); }
}