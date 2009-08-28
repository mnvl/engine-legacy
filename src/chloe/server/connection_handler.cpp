
#include "connection_handler.h"

namespace chloe
{

void ConnectionHandler::on_connect(boost::shared_ptr<rpc::Connection> const &connection_ptr)
{
}

void ConnectionHandler::on_read(boost::shared_ptr<rpc::Connection> const &connection_ptr, boost::uint16_t id, rpc::Buffer &buf)
{
}

void ConnectionHandler::on_error(boost::shared_ptr<rpc::Connection> const &connection_ptr, boost::system::error_code const &error)
{
}

}
