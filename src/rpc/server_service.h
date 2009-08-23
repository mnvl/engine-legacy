#pragma once

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread/thread.hpp>
#include "handler.h"
#include "basic_service.h"

namespace rpc
{

class Connection;

class ServerService:
		public BasicService,
		public boost::enable_shared_from_this<ServerService>
{
public:
	ServerService(Handler *handler_ptr);
	~ServerService();

	void start(std::string const &host_name, std::string const &service_name, size_t nthreads);

private:
	Handler *handler_;
	boost::asio::ip::tcp::acceptor acceptor_;

	void schedule_accept();
	void handle_accept(boost::shared_ptr<Connection> const &connection_ptr,
		boost::system::error_code const &error);
};

}
