#pragma once

#include <boost/cstdint.hpp>
#include <rpc/connection.h>
#include <rpc/buffer.h>

namespace chloe
{

class ServiceHandler
{
public:
	static void handle_handshake(boost::shared_ptr<rpc::Connection> const &connection_ptr, rpc::Buffer &buf);
	static void handle_time_sync(boost::shared_ptr<rpc::Connection> const &connection_ptr, rpc::Buffer &buf);
	static void handle_logic_error(boost::shared_ptr<rpc::Connection> const &connection_ptr, rpc::Buffer &buf);
	static void handle_internal_error(boost::shared_ptr<rpc::Connection> const &connection_ptr, rpc::Buffer &buf);
};

}