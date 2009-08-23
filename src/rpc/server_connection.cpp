
#include <iosfwd>
#include <boost/bind.hpp>
#include <boost/thread/locks.hpp>
#include "header.h"
#include "server_service.h"
#include "server_connection.h"

namespace rpc
{

ServerConnection::ServerConnection(Handler *Handler, boost::asio::io_service &io_service):
	handler_(Handler),
	strand_(io_service),
	socket_(io_service)
{
}

void ServerConnection::start()
{
	read(boost::shared_ptr<Buffer>(new Buffer()));
}

void ServerConnection::read(boost::shared_ptr<Buffer> const &buffer_ptr)
{
	if (buffer_ptr->write_bytes_available() < MINIMAL_ASYNC_READ_BUFFER_SIZE)
	{
		buffer_ptr->enlarge();
	}

	socket_.async_read_some(boost::asio::buffer(buffer_ptr->write_ptr(), buffer_ptr->write_bytes_available()),
		strand_.wrap(boost::bind(&ServerConnection::handle_read, shared_from_this(), buffer_ptr,
				boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error)));
}

void ServerConnection::write(boost::shared_ptr<Buffer> const &buffer_ptr)
{
	bool no_write_in_progress = false;

	{
		boost::unique_lock<boost::mutex> lock(write_queue_mutex_);
		no_write_in_progress = write_queue_.empty();
		write_queue_.push_back(buffer_ptr);
	}

	if (no_write_in_progress)
	{
		schedule_write();
	}
}

void ServerConnection::schedule_write()
{
	boost::shared_ptr<Buffer> buffer_ptr;

	{
		boost::unique_lock<boost::mutex> lock(write_queue_mutex_);
		buffer_ptr = write_queue_.front();
	}

	boost::asio::async_write(socket_,
		boost::asio::buffer(buffer_ptr->read_ptr(), buffer_ptr->read_bytes_available()),
		strand_.wrap(boost::bind(&ServerConnection::handle_write, shared_from_this(),
		boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error)));
}

void ServerConnection::handle_read(boost::shared_ptr<Buffer> const &buffer_ptr,
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
	catch(Handler::CloseServerConnection const &)
	{
		return;
	}

	buffer_ptr->set_read(h.len);
	buffer_ptr->set_write(write_offset_saved);
	buffer_ptr->consume(h.len);

	read(buffer_ptr);
}

void ServerConnection::handle_write(size_t bytes_transferred, boost::system::error_code const &error)
{
	boost::shared_ptr<Buffer> buffer_ptr;

	bool more_writes_pending;

	{
		boost::unique_lock<boost::mutex> lock(write_queue_mutex_);
		buffer_ptr = write_queue_.front();
		write_queue_.pop_front();

		more_writes_pending = !write_queue_.empty();
	}

	if (more_writes_pending)
	{
		schedule_write();
	}

	if (error)
	{
		handler_->error(this, error);
	}
}

}
