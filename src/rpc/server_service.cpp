
#include <boost/bind.hpp>
#include <util/logger.h>
#include "server_connection.h"
#include "server_service.h"

namespace rpc
{

ServerService::ServerService(Handler *handler_ptr):
	handler_(handler_ptr),
    acceptor_(get_io_service())
{
}

ServerService::~ServerService()
{
}

void ServerService::start(std::string const &host_name, std::string const &service_name, size_t nthreads)
{
	boost::asio::ip::tcp::resolver resolver(io_service_);
	boost::asio::ip::tcp::resolver::query query(host_name, service_name);
	boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);

	acceptor_.open(endpoint.protocol());
	acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	acceptor_.bind(endpoint);
	acceptor_.listen();

	schedule_accept();

    start(nthreads);

	LOG_INFO("service on host " << host_name << ":" << service_name << " up and running in "
		<< nthreads << " threads");
}

void ServerService::stop()
{
	io_service_.stop();
}

void ServerService::wait()
{
	threads_.join_all();
}

void ServerService::schedule_accept()
{
	boost::shared_ptr<ServerConnection> connection_ptr(new ServerConnection(handler_, io_service_));

   	acceptor_.async_accept(connection_ptr->socket(),
		boost::bind(&ServerService::handle_accept, this, connection_ptr, boost::asio::placeholders::error));
}

void ServerService::handle_accept(boost::shared_ptr<ServerConnection> const &connection_ptr,
	boost::system::error_code const &error)
{
	schedule_accept();
	connection_ptr->start();
}

}
