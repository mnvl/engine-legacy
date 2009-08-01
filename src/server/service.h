#pragma once

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread/thread.hpp>
#include "handler.h"

namespace server
{

class connection;

class service:
		public boost::noncopyable,
		public boost::enable_shared_from_this<service>
{
public:
	service(handler *handler_ptr);
	~service();

	void start(std::string const &host_name, std::string const &service_name, size_t nthreads);
	void stop();
	void wait();

private:
	handler *handler_;
	boost::asio::io_service io_service_;
	boost::asio::ip::tcp::acceptor acceptor_;
	boost::thread_group threads_;

	void schedule_accept();
	void handle_accept(boost::shared_ptr<connection> const &connection_ptr,
		boost::system::error_code const &error);
};

}
