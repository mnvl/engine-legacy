#pragma once

#include <rpc/handler.h>

namespace starmastersd
{

class ConnectionHandler: public rpc::Handler
{
public:
	void process(rpc::Connection *connection_ptr, boost::uint16_t id, rpc::Buffer &buf);
	void error(rpc::Connection *connection_ptr, boost::system::error_code const &error);
};

}
