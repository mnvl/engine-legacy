#pragma once

#include <iosfwd>
#include <boost/cstdint.hpp>
#include <boost/system/error_code.hpp>
#include "buffer.h"

namespace server
{

class connection;

class close_connection
{
};

class handler
{
public:
	virtual void process(connection *connection_ptr, boost::uint16_t id, buffer &buf) = 0;
	virtual void error(connection *connection_ptr, boost::system::error_code const &error) = 0;
};

}
