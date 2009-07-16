
#include <stdint.h>
#include <boost/bind.hpp>
#include "service.h"
#include "connection.h"

namespace server
{

connection::connection(boost::asio::io_service &io_service):
strand_(io_service),
socket_(io_service)
{
}

void connection::start()
{
	read();
}

void connection::read()
{
	boost::shared_ptr<boost::asio::streambuf> buffer(new boost::asio::streambuf());

	boost::asio::async_read(socket_, *buffer,
		boost::bind(connection::read_completed, boost::ref(*buffer)),
		strand_.wrap(boost::bind(&connection::handle_read, shared_from_this(),
				buffer, boost::asio::placeholders::error)));
}

bool connection::read_completed(boost::asio::streambuf &buffer)
{
	return false;
}

void connection::handle_read(boost::shared_ptr<boost::asio::streambuf> const &buffer,
		boost::system::error_code const &error)
{
	if (error)
	{
	}
	else
	{
	}
}

}
