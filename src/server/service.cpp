
#include <boost/bind.hpp>
#include <util/logger.h>
#include "connection.h"
#include "service.h"

namespace server
{

Service::Service(Handler *handler_ptr):
	handler_(handler_ptr),
	acceptor_(io_service_)
{
}

Service::~Service()
{
}

void Service::start(std::string const &host_name, std::string const &service_name, size_t nthreads)
{
	boost::asio::ip::tcp::resolver resolver(io_service_);
	boost::asio::ip::tcp::resolver::query query(host_name, service_name);
	boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);

	acceptor_.open(endpoint.protocol());
	acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	acceptor_.bind(endpoint);
	acceptor_.listen();

	schedule_accept();

	for (size_t i = 0; i < nthreads; ++i)
	{
		threads_.create_thread(boost::bind(&boost::asio::io_service::run, &io_service_));
	}

	LOG_INFO("service on host " << host_name << ":" << service_name << " up and running in "
		<< nthreads << " threads");
}

void Service::stop()
{
	io_service_.stop();
}

void Service::wait()
{
	threads_.join_all();
}

void Service::schedule_accept()
{
	boost::shared_ptr<Connection> connection_ptr(new Connection(handler_, io_service_));

   	acceptor_.async_accept(connection_ptr->socket(),
		boost::bind(&Service::handle_accept, this, connection_ptr, boost::asio::placeholders::error));
}

void Service::handle_accept(boost::shared_ptr<Connection> const &connection_ptr,
	boost::system::error_code const &error)
{
	schedule_accept();
	connection_ptr->start();
}


}
