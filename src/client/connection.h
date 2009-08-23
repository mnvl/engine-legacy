#pragma once

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <util/buffer.h>

namespace client
{

class Service;

class Connection:
        public boost::noncopyable,
		public boost::enable_shared_from_this<Connection>
{
public:
    Connection(Service *service);
    ~Connection();

    void connect(std::string const &host = "localhost", std::string const &port = "17171");
    void read(

private:
    Service *service_;

    void handle_connect(const boost::system::error_code& error, boost::asio::tcp::resolver::iterator endpoint_iterator)
};

}
