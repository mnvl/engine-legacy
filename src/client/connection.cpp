
#include "service.h"
#include "connection.h"

namespace client
{

Connection::Connection(Service *service):
    service_(service)
{
}

Connection::~Connection()
{
}

void Connection::connect(std::string const &host, std::string const &port)
{
    boost::asio::tcp::resolver resolver(service_->get_io_service());
    boost::asio::tcp::resolver::query query(host, port);
    boost::asio::tcp::resolver::iterator endpoint_it = resolver.resolve(query);

    tcp::endpoint endpoint = *endpoint_it++;

    socket_.async_connect(endpoint,
        boost::bind(&Connection::handle_connect, shared_from_this(),
            boost::asio::placeholders::error, endpoint_it));
}

void Connection::handle_connect(const boost::system::error_code& error, boost::asio::tcp::resolver::iterator endpoint_it)
{
    if (error)
    {
        if (endpoint_iterator != tcp::resolver::iterator())
        {
            socket_.close();
            tcp::endpoint endpoint = *endpoint_it++;
            socket_.async_connect(endpoint,
                boost::bind(&Connection::handle_connect, shared_from_this(), boost::asio::placeholders::error, endpoint_it));
        }

        return;
    }

    read(boost::shared_ptr<util::Buffer>(new util::Buffer()));
}

}
