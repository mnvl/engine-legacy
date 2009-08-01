#pragma once

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "handler.h"
#include "buffer.h"

namespace server
{

class connection:
		public boost::noncopyable,
		public boost::enable_shared_from_this<connection>
{
public:
	connection(handler *handler, boost::asio::io_service &io_service);

	boost::asio::ip::tcp::socket &socket()
	{
		return socket_;
	}

	void start();
	void write(boost::shared_ptr<buffer> const &buffer_ptr);

private:
	static size_t const MINIMAL_ASYNC_READ_BUFFER_SIZE = 1024;

	handler *handler_;
	boost::asio::io_service::strand strand_;
	boost::asio::ip::tcp::socket socket_;

	void read(boost::shared_ptr<buffer> const &buffer_ptr);
	void handle_read(boost::shared_ptr<buffer> const &buffer_ptr,
		size_t bytes_transferred, boost::system::error_code const &error);
	void handle_write(boost::shared_ptr<buffer> const &buffer_ptr,
		size_t bytes_transferred, boost::system::error_code const &error);
};

}
