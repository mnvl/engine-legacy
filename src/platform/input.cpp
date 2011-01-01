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
#include <platform/input.h>

namespace platform
{

Input::Input(boost::shared_ptr<Window> const &window_ptr):
	window_(window_ptr)
{
	HRESULT hr;

	IDirectInput8 *di;
	hr = DirectInput8Create(GetModuleHandle(0),DIRECTINPUT_VERSION,
		IID_IDirectInput8,(void**) &di,0);
	if(FAILED(hr)) throw exception::directx_error(hr);
	driver_.reset(di);
    di->Release();
}

Input::~Input() {
}

}
