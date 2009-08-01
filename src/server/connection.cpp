
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
	read(boost::shared_ptr<buffer>(new buffer()));
}

void connection::read(boost::shared_ptr<buffer> const &buffer_ptr)
{
	if (buffer_ptr->write_bytes_available() < MINIMAL_ASYNC_READ_BUFFER_SIZE)
	{
		buffer_ptr->enlarge();
	}

	socket_.async_read_some(boost::asio::buffer(buffer_ptr->write_ptr(), buffer_ptr->write_bytes_available()),
		strand_.wrap(boost::bind(&connection::handle_read, shared_from_this(), buffer_ptr,
				boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error)));
}

void connection::write(boost::shared_ptr<buffer> const &buffer_ptr)
{
	boost::asio::async_write(socket_,
		boost::asio::buffer(buffer_ptr->read_ptr(), buffer_ptr->read_bytes_available()),
		strand_.wrap(boost::bind(&connection::handle_write, shared_from_this(), buffer_ptr,
				boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error)));
}

void connection::handle_read(boost::shared_ptr<buffer> const &buffer_ptr,
	size_t bytes_transferred, boost::system::error_code const &error)
{
	if (error)
	{
		return handler_->error(this, error);
	}

	buffer_ptr->seek_write(bytes_transferred);

	if (bytes_transferred == 0 ||
		buffer_ptr->read_bytes_available() < sizeof(header) ||
		reinterpret_cast<header *>(buffer_ptr->read_ptr())->len > buffer_ptr->read_bytes_available())
	{
		return read(buffer_ptr);
	}

	header h;
	buffer_ptr->read((char *) &h, sizeof(h));

	size_t write_offset_saved = buffer_ptr->write_offset();
	buffer_ptr->set_write(h.len);

	try
	{
		handler_->process(this, h.id, *buffer_ptr);
	}
	catch(close_connection const &)
	{
		return;
	}

	buffer_ptr->set_read(h.len);
	buffer_ptr->set_write(write_offset_saved);
	buffer_ptr->consume(h.len);

	read(buffer_ptr);
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
