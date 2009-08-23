#pragma once

#include <list>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread/mutex.hpp>
#include <util/buffer.h>
#include "handler.h"

namespace server
{

class Connection:
		public boost::noncopyable,
		public boost::enable_shared_from_this<Connection>
{
public:
	Connection(Handler *handler, boost::asio::io_service &io_service);

	boost::asio::ip::tcp::socket &socket()
	{
		return socket_;
	}

	void start();
	void write(boost::shared_ptr<util::Buffer> const &buffer_ptr);

private:
	static size_t const MINIMAL_ASYNC_READ_BUFFER_SIZE = 1024;

	Handler *handler_;
	boost::asio::io_service::strand strand_;
	boost::asio::ip::tcp::socket socket_;

	boost::mutex write_queue_mutex_;
	std::list<boost::shared_ptr<util::Buffer> > write_queue_;

	void schedule_write();
	void read(boost::shared_ptr<util::Buffer> const &buffer_ptr);
	void handle_read(boost::shared_ptr<util::Buffer> const &buffer_ptr,
		size_t bytes_transferred, boost::system::error_code const &error);
	void handle_write(size_t bytes_transferred, boost::system::error_code const &error);
};

}
