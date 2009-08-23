#pragma once

#include "basic_connection.h"

namespace rpc
{

class ServerConnection:
		public BasicConnection
{
public:
	ServerConnection(Handler *handler, boost::asio::io_service &io_service);

	boost::asio::ip::tcp::socket &socket() { return socket_; }
    boost::asio::ip::tcp::socket const &socket() const { return socket_; }

private:
	Handler *handler_;

	virtual void handle_read(boost::shared_ptr<Buffer> const &buffer_ptr,
		size_t bytes_transferred, boost::system::error_code const &error);
};

}
