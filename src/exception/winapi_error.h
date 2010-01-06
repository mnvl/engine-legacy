//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#pragma once

#include <exception>
#include <windows.h>

namespace exception {

class winapi_error: public std::exception {
private:
	DWORD error_code_;
	mutable char const *message_buffer_;

public:
	winapi_error() throw():
		error_code_(GetLastError()),
		message_buffer_(0)
	{
	}

	~winapi_error() throw() {
		if (message_buffer_) LocalFree((LPVOID) message_buffer_);
	}
	char const *what() const throw() {
		if(!message_buffer_) {
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,
				0,error_code_,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR) &message_buffer_,0,0);
		}

		return message_buffer_;
	}
};

}
