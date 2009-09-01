#pragma once

#include <rpc/handler.h>

namespace drive
{
namespace client
{

class ConnectionHandler: public rpc::Handler
{
public:
    void on_connect(boost::shared_ptr<rpc::Connection> const &connection_ptr);
	void on_read(boost::shared_ptr<rpc::Connection> const &connection_ptr, boost::uint16_t id, rpc::Buffer &buf);
	void on_error(boost::shared_ptr<rpc::Connection> const &connection_ptr, boost::system::error_code const &error);
};

}
}
