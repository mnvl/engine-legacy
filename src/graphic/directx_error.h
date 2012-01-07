#pragma once
//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2012 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <windows.h>
#include <exception>

namespace graphic {

class directx_error: public std::exception {
private:
	HRESULT hresult_;

public:
	directx_error(HRESULT hr) throw(): hresult_(hr) {}
	~directx_error() throw() {}
	char const *what() const throw() { return ""/* DXGetErrorDescription(hresult_)*/; }
};

}
