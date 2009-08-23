#pragma once

#include <server/handler.h>

namespace starmastersd
{

class ConnectionHandler: public server::Handler
{
public:
	void process(server::Connection *connection_ptr, boost::uint16_t id, server::Buffer &buf);
	void error(server::Connection *connection_ptr, boost::system::error_code const &error);
};

}