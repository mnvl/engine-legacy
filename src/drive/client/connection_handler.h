#pragma once

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <rpc/handler.h>

namespace drive
{
namespace client
{

class ConnectionHandler:
	public rpc::Handler,
	public boost::noncopyable
{
public:
	ConnectionHandler();
	~ConnectionHandler();

	void connect(std::string const &host, std::string const &port);
	void stop();

private:
	class Impl;
	boost::scoped_ptr<Impl> impl_;

    void on_connect(boost::shared_ptr<rpc::Connection> const &connection_ptr);
	void on_read(boost::shared_ptr<rpc::Connection> const &connection_ptr, boost::uint16_t id, rpc::Buffer &buf);
	void on_error(boost::shared_ptr<rpc::Connection> const &connection_ptr, boost::system::error_code const &error);
};

}
}
