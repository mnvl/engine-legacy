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

#include <iosfwd>
#include <boost/cstdint.hpp>
#include <boost/system/error_code.hpp>
#include <boost/shared_ptr.hpp>
#include "buffer.h"

namespace rpc
{

class Connection;

class Handler
{
public:
    struct CloseConnectionException {};

    virtual void on_connect(boost::shared_ptr<Connection> const &connection_ptr) = 0;
	virtual void on_read(boost::shared_ptr<Connection> const &connection_ptr, boost::uint16_t id, Buffer &in) = 0;
	virtual void on_error(boost::shared_ptr<Connection> const &connection_ptr, boost::system::error_code const &error) = 0;
};

}
