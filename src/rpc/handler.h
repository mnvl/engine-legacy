#pragma once

#include <iosfwd>
#include <boost/cstdint.hpp>
#include <boost/system/error_code.hpp>
#include "buffer.h"

namespace rpc
{

class ServerConnection;

class Handler
{
public:
	class CloseServerConnection {};

	virtual void process(ServerConnection *connection_ptr, boost::uint16_t id, Buffer &buf) = 0;
	virtual void error(ServerConnection *connection_ptr, boost::system::error_code const &error) = 0;
};

}
