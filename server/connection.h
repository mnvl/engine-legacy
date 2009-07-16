#pragma once

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace server
{

class Service;

class connection:
		public boost::noncopyable,
		public boost::enable_shared_from_this<connection>
{
public:
	connection(boost::asio::io_service &io_service);

	boost::asio::ip::tcp::socket &socket()
	{
		return socket_;
	}

	void start();

private:
	boost::asio::io_service::strand strand_;
	boost::asio::ip::tcp::socket socket_;

	void read();
	static bool read_completed(boost::asio::streambuf &buffer);
	void handle_read(boost::shared_ptr<boost::asio::streambuf> const &buffer,
		boost::system::error_code const &error);
};

}
