
#include <boost/bind.hpp>
#include "connection.h"
#include "service.h"

namespace server
{

service::service():
acceptor_(io_service_)
{
}

service::~service()
{
}

void service::start(std::string const &host_name, std::string const &service_name, size_t nthreads)
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
}

void service::stop()
{
	io_service_.stop();
}

void service::wait()
{
	threads_.join_all();
}

void service::schedule_accept()
{
	boost::shared_ptr<connection> connection_ptr(new connection(io_service_));

   	acceptor_.async_accept(connection_ptr->socket(),
		boost::bind(&service::handle_accept, this, connection_ptr, boost::asio::placeholders::error));
}

void service::handle_accept(boost::shared_ptr<connection> const &connection_ptr,
	boost::system::error_code const &error)
{
	schedule_accept();
	connection_ptr->start();
}


}
