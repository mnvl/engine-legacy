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

#include <boost/noncopyable.hpp>
#include "forward.h"

namespace graphic
{

class Shader: public boost::noncopyable {
public:
	virtual ~Shader();

    virtual void load_from_file(std::string const &filename) = 0;
    virtual void load_from_buffer(char const *buffer, size_t len) = 0;

protected:
    Shader();
};

}
