
#include <iosfwd>
#include <boost/bind.hpp>
#include "header.h"
#include "service.h"
#include "connection.h"

namespace server
{

connection::connection(handler *handler, boost::asio::io_service &io_service):
	handler_(handler),
	strand_(io_service),
	socket_(io_service)
{
}

void connection::start()
{
	read(boost::shared_ptr<buffer>(new buffer(sizeof(header))));
}

void connection::read(boost::shared_ptr<buffer> const &buffer_ptr)
{
	if (buffer_ptr->write_bytes_available() < READ_BYTES)
	{
		buffer_ptr->enlarge();
	}

	boost::asio::async_read(socket_, buffer_ptr->write_ptr(),
		strand_.wrap(boost::bind(&connection::handle_read, shared_from_this(), buffer,
				boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error)));
}

void connection::write(boost::shared_ptr<buffer> const &buffer_ptr)
{
	boost::asio::async_write(socket_, buffer_ptr->read_ptr(),
		strand_.wrap(boost::bind(&connection::handle_write, shared_from_this(), buffer,
				boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error)));
}

void connection::handle_read(boost::shared_ptr<buffer> const &buffer_ptr,
	size_t bytes_transferred, boost::system::error_code const &error)
{
	if (error)
	{
		handler_->error(this, error);
		return;
	}

	std::istream is(buffer.get());

	header h;
	if (!is.read((char *) &h, sizeof(h)))
	{
		read(buffer);
		return;
	}

	if (h.len < buffer->size())
	{
		read(buffer);
		return;
	}

	try
	{
		handler_->process(this, h.id, h.len - sizeof(header), is);
	}
	catch(close_connection const &)
	{
		return;
	}

	buffer->consume(h.len);
	read(buffer);
}

void connection::handle_write(boost::shared_ptr<buffer> const &buffer_ptr,
	size_t bytes_transferred, boost::system::error_code const &error)
{
	if (error)
	{
		handler_->error(this, error);
	}
}

}
